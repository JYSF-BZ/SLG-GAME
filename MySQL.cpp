#include "MySQL.h"
#include"MySQL.h"
#define HOST "localhost"
#define USER "root"
#define PASS "jysf04111"
#define PORT 3306
MYSQL* conn = nullptr;

DataWarehouse DataWarehouse::instance;

int DataWarehouse::SQLiteSelectCallback(void* arg, int col_cnt, char** col_val, char** col_name){
	DataWarehouse* dw = static_cast<DataWarehouse*>(arg);
	// 第一行：存储字段名
	if (dw->sqlite_field_names.empty()){
		for (int i = 0; i < col_cnt; i++){
			dw->sqlite_field_names.emplace_back(col_name[i] ? col_name[i] : "");
		}
	}
	// 存储当前行数据
	std::vector<std::string> row;
	for (int i = 0; i < col_cnt; i++){
		row.emplace_back(col_val[i] ? col_val[i] : "NULL");
	}
	dw->sqlite_result_data.push_back(row);
	return 0;
}

std::string DataWarehouse::AdaptSqlSyntax(const std::string& sql){
	std::string res = sql;
	size_t pos = 0;
	// auto_increment 替换为 SQLite 的 autoincrement
	while ((pos = res.find("auto_increment", pos)) != std::string::npos){
		res.replace(pos, 14, "autoincrement");
		pos += 12;
	}
	return res;
}

void DataWarehouse::GetPrimaryKey(const std::string& TarTable) {
	if (!is_mysql_available){
		PrimaryKey.clear();
		std::cout << "SQLite模式，主键列表置空" << std::endl;
		return;
	}
	std::string safe_name = SafeName(TarTable);
	std::string sql = "show keys from " + safe_name + " where Key_name='PRIMARY'";
	RollSQL(sql.c_str());
	MYSQL_RES* res = mysql_store_result(conn);
	if (!res) {
		return;
	}
	int col_num = mysql_num_fields(res);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res))) {
		if (row[4] != nullptr) {
			PrimaryKey.push_back(row[4]);
		}
	}
	mysql_free_result(res);
}

void DataWarehouse::RollSQL(const char* tarindication) {
	if (!tarindication || std::string(tarindication).empty()) {
		return;
	}
	std::string sql = AdaptSqlSyntax(tarindication);

	if (is_mysql_available){
		// MySQL执行分支
		if (!mysql_query(conn, sql.c_str())) {
			std::cout << "[MySQL] " << sql << " 执行成功" << std::endl;
		}
		else {
			std::cout << "[MySQL 错误]：" << mysql_error(conn) << std::endl;
			std::cout << sql << " 执行失败" << std::endl;
		}
	}
	else{
		// SQLite执行分支：先清空上次查询缓存
		sqlite_result_data.clear();
		sqlite_field_names.clear();
		char* err_msg = nullptr;
		int ret = sqlite3_exec(sqlite_conn, sql.c_str(), SQLiteSelectCallback, this, &err_msg);
		if (ret == SQLITE_OK){
			std::cout << "[SQLite内存模拟] " << sql << " 执行成功" << std::endl;
		}
		else{
			std::cout << "[SQLite 错误]：" << (err_msg ? err_msg : "未知错误") << std::endl;
			std::cout << sql << " 执行失败" << std::endl;
			sqlite3_free(err_msg);
		}
	}
}

void DataWarehouse::Init() {
	conn = mysql_init(NULL);
}

bool DataWarehouse::InitUser() {
	SetConsoleOutputCP(65001);
	if (!conn){
		std::cout << "MySQL初始化失败" << std::endl;
		return false;
	}
	if (!mysql_real_connect(conn, HOST, USER, PASS, NULL, PORT, NULL, 0)) {
		std::cout << "MySQL连接失败：" << mysql_error(conn) << std::endl;
		return false;
	}
	std::cout << "MySQL连接成功" << std::endl;
	if (mysql_query(conn, "SET NAMES utf8")){
		std::cout << "字符集设置失败：" << mysql_error(conn) << std::endl;
	}
	return true;
}

std::string DataWarehouse::Escape(const std::string& input) {
	if (!is_mysql_available || !conn || input.empty())
		return input;

	std::string output;
	output.resize(input.size() * 2);

	size_t len = mysql_real_escape_string(
		conn,
		&output[0],
		input.c_str(),
		input.size()
	);

	output.resize(len);
	return output;
}

std::string DataWarehouse::SafeName(const std::string& name) {
	if (!conn || name.empty())
		return "";

	std::string res;
	// 遍历每个字符，只保留安全字符，删掉反引号 `
	for (char c : name) {
		if (c == '`') continue; // 绝对禁止反引号，防止闭合
		res += c;
	}
	// 前后加上反引号，变成 `安全表名`
	return "`" + res + "`";
}

void DataWarehouse::Change() {
	if (PrimaryKey.size() != 0) {
		for (auto& token : PrimaryKey) {
			token = SafeName(token);
		}
	}
	if (ColumnName.size() != 0) {
		for (auto& token : ColumnName) {
			token = SafeName(token);
		}
	}
	if (Title.size() != 0) {
		for (auto& token : Title) {
			token = SafeName(token);
		}
	}
	if (Value.size() != 0) {
		for (auto& token : Value) {
			for (auto& token1 : token) {
				token1 = Escape(token1);
			}
		}
	}
	if (Column.size() != 0) {
		for (int i = 0; i < Column.size(); i++) {
			if (Column[i].size() != 0) {
				if (Column[i][0] == "0" || Column[i][0] == "2" || Column[i][0] == "3" || Column[i][0] == "4") {
					for (int j = 1; j < Column[i].size(); j++) {
						Column[i][j] = SafeName(Column[i][j]);
					}
				}
				else if (Column[i][0] == "1") {
					for (int j = 2; j < Column[i].size(); j++) {
						Column[i][j] = SafeName(Column[i][j]);
					}
				}
				else if (Column[i][0] == "10") {
					for (int j = 2; j < Column[i].size(); j++) {
						Column[i][j] = SafeName(Column[i][j]);
					}
				}
			}
		}
	}
	if (As.size() != 0) {
		for (auto& token : As) {
			token = Escape(token);
		}
	}
	if (OrderByColumn.size() != 0) {
		for (auto& token : OrderByColumn) {
			token = SafeName(token);
		}
	}
	if (TarColumn.size() != 0) {
		for (int i = 0; i < TarColumn.size(); i++) {
			if (TarColumn[i].size() != 0) {
				if (i == 0) {
					for (int j = 0; j < TarColumn[i].size(); j++) {
						TarColumn[i][j] = SafeName(TarColumn[i][j]);
					}
				}
				else if (TarColumn[i][0] == "0" || TarColumn[i][0] == "1" || TarColumn[i][0] == "2" || TarColumn[i][0] == "3" || TarColumn[i][0] == "4") {
					for (int j = 1; j < TarColumn[i].size(); j++) {
						TarColumn[i][j] = SafeName(TarColumn[i][j]);
					}
				}
				else if (TarColumn[i][0] == "10") {
					for (int j = 2; j < TarColumn[i].size(); j++) {
						TarColumn[i][j] = SafeName(TarColumn[i][j]);
					}
				}
			}
		}
	}
	if (Condition.size() != 0) {
		for (int i = 0; i < Condition.size(); i++) {
			for (int j = 0; j < Condition[i].size(); j++) {
				Condition[i][j] = Escape(Condition[i][j]);
			}
		}
	}
	if (GroupBy.size() != 0) {
		for (int i = 0; i < GroupBy.size(); i++) {
			GroupBy[i] = SafeName(GroupBy[i]);
		}
	}
}

void DataWarehouse::Clear() {
	PrimaryKey.clear();
	ColumnName.clear();
	ColumnType.clear();
	ColumnB.clear();
	ColumnIsPrimaryKey.clear();
	ColumnIsNotNull.clear();
	ColumnIsUnique.clear();
	ColumnIsAuto_increasement.clear();
	Title.clear();
	Value.clear();
	Column.clear();
	As.clear();
	OrderByColumn.clear();
	OrderByIsAsc.clear();
	TarColumn.clear();
	Condition.clear();
	OP.clear();
	Connecter.clear();
	GroupBy.clear();
	Limit.clear();
}

void DataWarehouse::DisplayData() {
	if (is_mysql_available){
		MYSQL_RES* res = mysql_store_result(conn);
		if (!res) {
			return;
		}
		int col_num = mysql_num_fields(res);
		MYSQL_FIELD* fields = mysql_fetch_fields(res);
		const int COLUMN_WIDTH = 15;
		for (int i = 0; i < col_num; i++) {
			printf("%-*s", COLUMN_WIDTH, fields[i].name);
		}
		std::cout << std::endl;
		for (int i = 0; i < col_num; i++) {
			for (int j = 0; j < COLUMN_WIDTH; j++) {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res))) {
			for (int i = 0; i < col_num; i++) {
				if (row[i] == nullptr) {
					printf("%-*s", COLUMN_WIDTH, "NULL");
				}
				else {
					printf("%-*s", COLUMN_WIDTH, row[i]);
				}
			}
			std::cout << std::endl;
		}
		mysql_free_result(res);
	}
	else{
		const int COLUMN_WIDTH = 15;
		for (const auto& name : sqlite_field_names){
			printf("%-*s", COLUMN_WIDTH, name.c_str());
		}
		std::cout << "\n";
		for (size_t i = 0; i < sqlite_field_names.size(); ++i){
			std::cout << std::string(COLUMN_WIDTH, '-');
		}
		std::cout << "\n";
		for (const auto& row : sqlite_result_data){
			for (const auto& val : row){
				printf("%-*s", COLUMN_WIDTH, val.c_str());
			}
			std::cout << "\n";
		}
	}
}

DataWarehouse::DataWarehouse() {
	Init();
	is_mysql_available = InitUser();
	if (!is_mysql_available) {
		std::cout << "MySQL不可用" << std::endl;
		if (sqlite3_open(":memory:", &sqlite_conn) != SQLITE_OK){
			std::cout << "SQLite内存数据库打开失败:" << sqlite3_errmsg(sqlite_conn) << std::endl;
		}
	}
}

DataWarehouse::~DataWarehouse(){
	if (conn) {
		mysql_close(conn);
		conn = nullptr;
	}
	if (!is_mysql_available && sqlite_conn) {
		sqlite3_close(sqlite_conn);
	}
}

std::string DataWarehouse::SaveAdd() {
	std::string sql = "";
	for (int i = 0; i < ColumnName.size(); i++) {
		sql = sql + " add " + ColumnName[i] + " " + ColumnType[i];
		if (ColumnType[i] == "varchar" || ColumnType[i] == "char") {
			sql = sql + " (" + std::to_string(ColumnB[i]) + ")";
		}
		if (ColumnIsNotNull[i]) {
			sql = sql + " not null";
		}
		if (ColumnIsUnique[i]) {
			sql = sql + " unique";
		}
		if (ColumnIsAuto_increasement[i]) {
			sql += " auto_increment";
		}
		if (i < ColumnName.size() - 1) {
			sql += ",";
		}
	}
	return sql;
}

std::string DataWarehouse::SaveChange() {
	std::string sql = "";
	for (int i = 0; i < ColumnName.size(); i++) {
		sql = sql + " change " + ColumnName[i] + " " + TarColumn[0][i] + " " + ColumnType[i];
		if (ColumnType[i] == "varchar" || ColumnType[i] == "char") {
			sql = sql + " (" + std::to_string(ColumnB[i]) + ")";
		}
		if (ColumnIsNotNull[i]) {
			sql = sql + " not null";
		}
		if (ColumnIsUnique[i]) {
			sql = sql + " unique";
		}
		if (ColumnIsAuto_increasement[i]) {
			sql += " auto_increment";
		}
		if (i < ColumnName.size() - 1) {
			sql += ",";
		}
	}
	return sql;
}

std::string DataWarehouse::SaveType() {
	std::string sql = "";
	for (int i = 0; i < ColumnName.size(); i++) {
		sql = sql + " modify " + ColumnName[i] + " " + ColumnType[i];
		if (ColumnType[i] == "varchar" || ColumnType[i] == "char") {
			sql = sql + " (" + std::to_string(ColumnB[i]) + ")";
		}
		if (ColumnIsNotNull[i]) {
			sql = sql + " not null";
		}
		if (ColumnIsUnique[i]) {
			sql = sql + " unique";
		}
		if (ColumnIsAuto_increasement[i]) {
			sql += " auto_increment";
		}
		if (i < ColumnName.size() - 1) {
			sql += ",";
		}
	}
	return sql;
}

std::string DataWarehouse::SavePrimaryKey() {
	std::string sql = "";
	for (int i = 0; i < ColumnName.size(); i++) {
		sql = sql + ColumnName[i];
		if (i < ColumnName.size() - 1) {
			sql += ",";
		}
	}
	return sql;
}

std::string DataWarehouse::IsDistinct() {
	std::string sql = "";
	if (flag_distinct) {
		sql = sql + "distinct ";
	}
	return sql;
}

std::string DataWarehouse::IsIgnore() {
	std::string sql = "";
	if (flag_ignore) {
		if (is_mysql_available)
			sql = "ignore ";
		else
			sql = "or ignore ";
	}
	return sql;
}

std::string DataWarehouse::HelpUpdate() {
	std::string sql = "";
	for (int i = 0; i < Value.size(); i++) {
		sql = sql + " " + Column[0][i] + "='" + Value[0][i] + "'";
		if (i < Value.size() - 1) {
			sql += ",";
		}
	}
	return sql;
}

std::string DataWarehouse::HelpWhere() {
	std::string sql = "";
	if (TarColumn.size() == 0) {
		return sql;
	}
	if (TarColumn[0].size() != 0) {
		sql += " where";
		for (int i = 0; i < TarColumn[0].size(); i++) {
			sql = sql + " " + TarColumn[0][i] + HelpHelpWhereHaving(0, i);
		}
	}
	return sql;
}

std::string DataWarehouse::HelpGroupBy() {
	std::string sql = "";
	if (GroupBy.size() == 0) {
		return sql;
	}
	sql = " group by ";
	std::unordered_set<std::string> group_set;
	for (int j = 0; j < GroupBy.size(); j++) {
		group_set.insert(GroupBy[j]);
	}
	if (Column.size() == 0) {
		return sql;
	}
	for (int i = 0; i < Column.size(); i++) {
		if (Column[i][0] == "0") {
			for (int j = 1; j < Column[i].size(); j++) {
				if (!group_set.count(Column[i][j])) {
					return sql;
				}
			}
		}
		else if (Column[i][0] == "10") {

		}
		else {
			return sql;
		}
	}
	for (int i = 0; i < GroupBy.size(); i++) {
		sql += GroupBy[i];
		if (i < GroupBy.size() - 1) {
			sql += ",";
		}
	}
	sql += HelpHaving(group_set);
	return sql;
}

std::string DataWarehouse::HelpHaving(const std::unordered_set<std::string>& Group_Set) {
	std::string sql = "";
	if (TarColumn.size() < 2 || TarColumn[1].size() == 0) {
		return sql;
	}
	sql += " having";
	for (int i = 1; i < TarColumn.size(); i++) {
		if (TarColumn[i].size() != 0 && TarColumn[i][0] == "0") {
			for (int j = 1; j < TarColumn[i].size(); j++) {
				if (!Group_Set.count(TarColumn[i][j])) {
					return sql;
				}
			}
		}
	}
	for (int i = 0; i < TarColumn.size(); i++) {
		if (TarColumn[i].size() == 0) {
			continue;
		}
		if (TarColumn[i][0] == "10") {
			if (TarColumn[i].size() == 1) {
				continue;
			}
			std::string sql0 = "";
			if (TarColumn[i][1] == "0") {
				sql0 += " count(";
			}
			else if (TarColumn[i][1] == "1") {
				sql0 += " max(";
			}
			else if (TarColumn[i][1] == "2") {
				sql0 += " min(";
			}
			else if (TarColumn[i][1] == "3") {
				sql0 += " avg(";
			}
			for (int j = 2; j < TarColumn[i].size(); j++) {
				if (TarColumn[i][j] == "") {
					sql = sql + sql0 + "*)";
				}
				else {
					sql = sql + sql0 + TarColumn[i][j] + ")";
				}
				sql += HelpHelpWhereHaving(i, j - 2);
			}
		}
		else if (TarColumn[i][0] == "0") {
			for (int j = 1; j < TarColumn[i].size(); j++) {
				sql = sql + " " + TarColumn[i][j];
				sql += HelpHelpWhereHaving(i, j - 1);
			}
		}
	}
	return sql;
}

std::string DataWarehouse::HelpOrder() {
	std::string sql = "";
	if (OrderByColumn.size() != 0) {
		sql += " order by ";
		for (int i = 0; i < OrderByColumn.size(); i++) {
			sql = sql + OrderByColumn[i] + " ";
			if (OrderByIsAsc[i]) {
				sql = sql + "asc";
			}
			else {
				sql = sql + "desc";
			}
			if (i != OrderByColumn.size() - 1) {
				sql += ",";
			}
		}
	}
	return sql;
}

std::string DataWarehouse::HelpLimit() {
	std::string sql = "";
	if (Limit.size() == 0) {
		return sql;
	}
	sql = sql + " limit " + std::to_string(Limit[0]) + "," + std::to_string(Limit[1]);
	return sql;
}

std::string DataWarehouse::HelpHelpWhereHaving(const int& x, const int& i) {
	std::string sql = "";
	if (OP[x][i] == "like") {
		sql = sql + " like '%" + Condition[x][i] + "%'";
	}
	else if (OP[x][i] == "%like") {
		sql = sql + " like '%" + Condition[x][i] + "'";
	}
	else if (OP[x][i] == "like%") {
		sql = sql + " like '" + Condition[x][i] + "%'";
	}
	else {
		sql = sql + OP[x][i] + "'" + Condition[x][i] + "'";
	}
	if (i < Condition[x].size() - 1) {
		if (Connecter[x][i]) {
			sql = sql + " and";
		}
		else {
			sql = sql + " or";
		}
	}
	return sql;
}

std::string DataWarehouse::HelpColumn() {
	std::string sql = "";
	if (Column.size() == 0) {
		sql += "*";
	}
	else {
		for (int i = 0; i < Column.size(); i++) {
			if (Column[i][0] == "0") {
				sql += HelpHelpColumn0(i);
			}
			else if (Column[i][0] == "1") {
				sql += HelpHelpColumn1(i);
			}
			else if (Column[i][0] == "2") {
				sql += HelpHelpColumn2(i);
			}
			else if (Column[i][0] == "3") {
				sql += HelpHelpColumn3(i);
			}
			else if (Column[i][0] == "4") {
				sql += HelpHelpColumn4(i);
			}
			else if (Column[i][0] == "10") {
				sql += HelpHelpColumn10(i);
			}
			if (i < Column.size() - 1) {
				sql += ",";
			}
		}
	}
	return sql;
}

std::string DataWarehouse::HelpHelpColumn0(const int& row) {
	std::string sql = "";
	for (int i = 1; i < Column[row].size(); i++) {
		sql = sql + Column[row][i];
		if (As.size() >= row + 1) {
			if (As[row] != "") {
				sql = sql + " as " + As[row];
			}
		}
		if (i < Column[row].size() - 1) {
			sql += ",";
		}
	}
	return sql;
}

std::string DataWarehouse::HelpHelpColumn1(const int& row) {
	std::string sql = "";
	if (Column[row].size() == 2) {
		sql = sql + Column[row][1];
	}
	else {
		std::string sel = Column[row][1];
		sql = sql + "concat(";
		for (int j = 2; j < Column[row].size(); j++) {
			sql = sql + Column[row][j];
			if (j < Column[row].size() - 1) {
				sql = sql + ",'" + sel + "',";
			}
		}
		sql = sql + ")";
	}
	if (As.size() >= row + 1) {
		if (As[row] != "") {
			sql = sql + " as " + As[row];
		}
	}
	return sql;
}

std::string DataWarehouse::HelpHelpColumn2(const int& row) {
	std::string sql = "";
	if (Column[row].size() == 2) {
		sql = sql + Column[row][1];
	}
	else {
		sql = sql + "substring(";
		for (int j = 1; j < Column[row].size(); j++) {
			if (j == 1) {
				sql = sql + Column[row][j];
			}
			else {
				sql = sql + "'" + Column[row][j] + "'";
			}
			if (j < Column[row].size() - 1) {
				sql = sql + ",";
			}
		}
		sql = sql + ")";
	}
	if (As.size() >= row + 1) {
		if (As[row] != "") {
			sql = sql + " as " + As[row];
		}
	}
	return sql;
}

std::string DataWarehouse::HelpHelpColumn3(const int& row) {
	std::string sql = "";
	sql = sql + "upper(" + Column[row][1] + ")";
	if (As.size() >= row + 1) {
		if (As[row] != "") {
			sql = sql + " as " + As[row];
		}
	}
	return sql;
}

std::string DataWarehouse::HelpHelpColumn4(const int& row) {
	std::string sql = "";
	sql = sql + "lower(" + Column[row][1] + ")";
	if (As.size() >= row + 1) {
		if (As[row] != "") {
			sql = sql + " as " + As[row];
		}
	}
	return sql;
}

std::string DataWarehouse::HelpHelpColumn10(const int& row) {
	std::string sql = "";
	std::string sql0 = "";
	if (Column[row][1] == "0") {
		sql0 = "count";
	}
	else if (Column[row][1] == "1") {
		sql0 = "max";
	}
	else if (Column[row][1] == "2") {
		sql0 = "min";
	}
	else if (Column[row][1] == "3") {
		sql0 = "avg";
	}
	if (Column[row].size() == 2) {
		sql = sql + sql0 + "(*)";
	}
	else {
		sql = sql + sql0 + "(" + Column[row][2] + ")";
	}
	if (As.size() >= row + 1) {
		if (As[row] != "") {
			sql = sql + " as " + As[row];
		}
	}
	return sql;
}

MYSQL* DataWarehouse::GetConn() {
	return conn;
}

std::vector<std::string>& DataWarehouse::GetColumnName() {
	return ColumnName;
}

std::vector<std::string>& DataWarehouse::GetColumnType() {
	return ColumnType;
}

std::vector<int>& DataWarehouse::GetColumnB() {
	return ColumnB;
}

std::vector<bool>& DataWarehouse::GetColumnIsPrimaryKey() {
	return ColumnIsPrimaryKey;
}

std::vector<bool>& DataWarehouse::GetColumnIsNotNull() {
	return ColumnIsNotNull;
}

std::vector<bool>& DataWarehouse::GetColumnIsUnique() {
	return ColumnIsUnique;
}

std::vector<bool>& DataWarehouse::GetColumnIsAuto_increasement() {
	return ColumnIsAuto_increasement;
}

std::vector<std::string>& DataWarehouse::GetTitle() {
	return Title;
}

std::vector<std::vector<std::string>>& DataWarehouse::GetValue() {
	return Value;
}

std::vector<std::vector<std::string>>& DataWarehouse::GetColumn() {
	return Column;
}

std::vector<std::string>& DataWarehouse::GetAs() {
	return As;
}

std::vector<std::string>& DataWarehouse::GetOrderByColumn() {
	return OrderByColumn;
}

std::vector<bool>& DataWarehouse::GetOrderByIsAsc() {
	return OrderByIsAsc;
}

std::vector<std::vector<std::string>>& DataWarehouse::GetTarColumn() {
	return TarColumn;
}

std::vector<std::vector<std::string>>& DataWarehouse::GetCondition() {
	return Condition;
}

std::vector<std::vector<std::string>>& DataWarehouse::GetOP() {
	return OP;
}

std::vector<std::vector<bool>>& DataWarehouse::GetConnecter() {
	return Connecter;
}

std::vector<std::string>& DataWarehouse::GetGroupBy() {
	return GroupBy;
}

std::vector<int>& DataWarehouse::GetLimit() {
	return Limit;
}

void DataWarehouse::UseTest(const std::string& TarTable) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持USE切换数据库，操作已忽略" << std::endl;
		return;
	}
	std::string safe_name = SafeName(TarTable);
	std::string sql = "use " + safe_name;
	RollSQL(sql.c_str());
}

void DataWarehouse::ShowCreateDataBaseSql(const std::string& TarTable) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持该操作，已忽略" << std::endl;
		return;
	}
	std::string safe_name = SafeName(TarTable);
	std::string sql = "show create database " + safe_name;
	RollSQL(sql.c_str());
}

void DataWarehouse::ShowDataBases() {
	if (!is_mysql_available){
		std::cout << "SQLite不支持查看数据库列表，已忽略" << std::endl;
		return;
	}
	RollSQL("show databases");
	DisplayData();
}

void DataWarehouse::ShowDataBase(const std::string& TarDataBase) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持该操作，已忽略" << std::endl;
		return;
	}
	std::string safe_name = Escape(TarDataBase);
	std::string sql = "show databases like '" + safe_name + "'";
	RollSQL(sql.c_str());
}

void DataWarehouse::CreateDataBase(const std::string& TarTable) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持创建数据库，已忽略" << std::endl;
		return;
	}
	std::string safe_name = SafeName(TarTable);
	std::string sql = "create database if not exists " + safe_name;
	RollSQL(sql.c_str());
}

void DataWarehouse::DropDataBase(const std::string& TarTable) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持删除数据库，已忽略" << std::endl;
		return;
	}
	std::string safe_name = SafeName(TarTable);
	std::string sql = "drop database if exists " + safe_name;
	RollSQL(sql.c_str());
}

void DataWarehouse::CreateTable(const std::string& TarTable) {
	Change();
	int ans = 0;
	bool pd = 0;
	std::string safe_name = SafeName(TarTable);
	std::string appendix = ",primary key(";
	for (int i = 0; i < ColumnIsPrimaryKey.size(); i++) {
		if (ColumnIsPrimaryKey[i]) {
			if (pd) {
				appendix += ",";
			}
			else {
				pd = 1;
			}
			appendix = appendix + ColumnName[i];
		}
	}
	appendix += ")";
	std::string sql = "create table " + safe_name + "(";
	for (int i = 0; i < ColumnName.size(); i++) {
		sql += ColumnName[i] + " " + ColumnType[i];
		if (ColumnType[i] == "varchar" || ColumnType[i] == "char") {
			sql += "(" + std::to_string(ColumnB[i]) + ")";
		}
		if (ColumnIsNotNull[i]) {
			sql += " not null";
		}
		if (ColumnIsUnique[i]) {
			sql += " unique";
		}
		if (ColumnType[i] == "int" && ColumnIsAuto_increasement[i]) {
			sql += " auto_increment";
		}
		if (i != ColumnName.size() - 1) {
			sql += ",";
		}
	}
	if (pd) {
		sql += appendix;
	}
	sql += ")";
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::ShowTable() {
	if (!is_mysql_available){
		std::cout << "SQLite不支持show tables语法，已忽略" << std::endl;
		return;
	}
	RollSQL("show tables");
	DisplayData();
}

void DataWarehouse::DescTable(const std::string& TarTable) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持desc查看表结构，已忽略" << std::endl;
		return;
	}
	std::string safe_name = SafeName(TarTable);
	std::string sql = "desc " + safe_name;
	RollSQL(sql.c_str());
	DisplayData();
}

void DataWarehouse::DropTable(const std::string& TarTable) {
	std::string safe_name = SafeName(TarTable);
	std::string sql = "drop table if exists " + safe_name;
	RollSQL(sql.c_str());
}

void DataWarehouse::AlterTableName(const std::string& TarTable, const std::string& NewTableName) {
	std::string oldsafe_name = SafeName(TarTable);
	std::string newsafe_name = SafeName(NewTableName);
	std::string sql = "alter table " + oldsafe_name + " rename to " + newsafe_name;
	RollSQL(sql.c_str());
}

void DataWarehouse::AddTableColumn(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "alter table " + safe_name + SaveAdd();
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::AlterTableColumn(const std::string& TarTable) {
	if (!is_mysql_available){
		std::cout << "SQLite不支持alter table change修改字段，已忽略" << std::endl;
		return;
	}
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "alter table " + safe_name + " " + SaveChange();
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::AlterTableColumnType(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "alter table " + safe_name + " " + SaveChange();
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::AlterAddPrimaryKey(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "alter table " + safe_name + " add primary key (" + SavePrimaryKey() + ")";
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::AlterDeletePrimaryKey(const std::string& TarTable) {
	Change();
	GetPrimaryKey(TarTable);
	std::string safe_name = SafeName(TarTable);
	if (PrimaryKey.size() == 1) {
		std::string sql1 = "alter table " + safe_name + " drop index " + PrimaryKey[0];
		RollSQL(sql1.c_str());
	}
	else if (PrimaryKey.size() == 0) {
		return;
	}
	std::string sql = "alter table " + safe_name + " drop primary key";
	RollSQL(sql.c_str());
	if (PrimaryKey.size() == 1) {
		std::string sql1 = "alter table " + safe_name + " add unique (" + PrimaryKey[0] + ")";
		RollSQL(sql1.c_str());
	}
	Clear();
}

void DataWarehouse::InsertRowData(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "insert " + IsIgnore() + " into " + safe_name + " (";
	for (int i = 0; i < Title.size(); i++) {
		sql += Title[i];
		if (i != Title.size() - 1) {
			sql += ",";
		}
	}
	sql += ") values";
	for (int i = 0; i < Value.size(); i++) {
		sql += "(";
		for (int j = 0; j < Value[i].size(); j++) {
			sql = sql + "'" + Value[i][j] + "'";
			if (j != Value[i].size() - 1) {
				sql += ",";
			}
		}
		sql += ")";
		if (i != Value.size() - 1) {
			sql += ",";
		}
	}
	RollSQL(sql.c_str());
	Clear();
	flag_ignore = false;
}

void DataWarehouse::DeleteRowData(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "delete from " + safe_name + HelpWhere();
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::UpdateRowData(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "update " + safe_name + " set" + HelpUpdate() + HelpWhere();
	RollSQL(sql.c_str());
	Clear();
}

void DataWarehouse::ShowRowData(const std::string& TarTable) {
	Change();
	std::string safe_name = SafeName(TarTable);
	std::string sql = "select " + IsDistinct() + HelpColumn() + " from " + safe_name + HelpWhere() + HelpGroupBy() + HelpOrder() + HelpLimit();
	RollSQL(sql.c_str());
	Clear();
	flag_distinct = false;
}

DataWarehouse& DataWarehouse::ShowRowDataDistinct() {
	flag_distinct = true;
	return *this;
}

DataWarehouse& DataWarehouse::InsertRowDataIgnore() {
	flag_ignore = true;
	return *this;
}

DataWarehouse& DataWarehouse::getInstance() {
	return instance;
}

bool DataWarehouse::IsHaveData() {
	if (is_mysql_available){
		if (!conn) {
			return false;
		}
		MYSQL_RES* res = mysql_store_result(conn);
		if (!res) {
			return false;
		}
		my_ulonglong row_num = mysql_num_rows(res);
		mysql_free_result(res);
		return (row_num > 0);
	}
	else{
		std::cout << "SQLite" << std::endl;
		return !sqlite_result_data.empty();
	}
}

bool DataWarehouse::IsMysqlAvailable()const{
	return is_mysql_available;
}

std::vector<std::string> DataWarehouse::GetFieldNames(){
	if (is_mysql_available){
		data.clear();
		MYSQL_RES* res = mysql_store_result(conn);
		if (!res) {
			data = {};
			return {};
		}
		int col = mysql_num_fields(res);
		MYSQL_FIELD* fields = mysql_fetch_fields(res);
		std::vector<std::string> names;
		for (int i = 0; i < col; ++i){
			names.emplace_back(fields[i].name);
		}

		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res))) {
			std::vector<std::string> tmp;
			unsigned int col = mysql_num_fields(res);
			for (unsigned int i = 0; i < col; ++i) {
				tmp.push_back(row[i] ? row[i] : "无");
			}
			data.push_back(tmp);
		}

		mysql_free_result(res);
		return names;
	}
	else{
		return sqlite_field_names;
	}
}

std::vector<std::vector<std::string>> DataWarehouse::GetResultRows(){
	if (is_mysql_available){
		return data;
	}
	else{
		auto tempData = sqlite_result_data;
		for (auto& r : tempData) {
			for (auto& val : r) {
				if (val == "NULL") {
					val = "";
				}
			}
		}
		return tempData;
	}
}
