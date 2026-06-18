#pragma once
#include<mysql.h>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<unordered_set>
#include"sqlite3.h"

class DataWarehouse {
private:
	bool flag_distinct = false;
	bool flag_ignore = false;
	bool is_mysql_available = false;

private:
	//临时data
	std::vector<std::vector<std::string>> data;
	// SQLite内存数据库句柄
	sqlite3* sqlite_conn = nullptr;
	// 缓存SQLite查询结果，用于模拟mysql_store_result
	std::vector<std::vector<std::string>> sqlite_result_data;
	std::vector<std::string> sqlite_field_names;
	// SQLite查询回调函数，用于接收查询结果
	static int SQLiteSelectCallback(void* arg, int col_cnt, char** col_val, char** col_name);
	std::string AdaptSqlSyntax(const std::string& sql);

private:
	//主键
	std::vector<std::string>PrimaryKey;
	//列变换
	std::vector<std::string>ColumnName;
	std::vector<std::string>ColumnType;
	std::vector<int>ColumnB;
	std::vector<bool>ColumnIsPrimaryKey;
	std::vector<bool>ColumnIsNotNull;
	std::vector<bool>ColumnIsUnique;
	std::vector<bool>ColumnIsAuto_increasement;
	//行变换
	std::vector<std::string>Title;
	std::vector<std::vector<std::string>>Value;
	//where以前的
	std::vector<std::vector<std::string>>Column;
	std::vector<std::string>As;
	//排序方式
	std::vector<std::string>OrderByColumn;
	std::vector<bool>OrderByIsAsc;
	//第一行是普通where条件语句，第二行是Having语句
	std::vector<std::vector<std::string>>TarColumn;
	std::vector<std::vector<std::string>>Condition;
	std::vector<std::vector<std::string>>OP;
	std::vector<std::vector<bool>>Connecter;
	//分组
	std::vector<std::string>GroupBy;
	//分页
	std::vector<int>Limit;

private:
	void GetPrimaryKey(const std::string& TarTable);
	void RollSQL(const char* tarindication);
	void Init();
	bool InitUser();
	std::string Escape(const std::string& input);
	std::string SafeName(const std::string& name);
	void Change();
	void Clear();
	//展示数据
	void DisplayData();

	DataWarehouse();
	~DataWarehouse();
	DataWarehouse(const DataWarehouse&) = delete;
	DataWarehouse& operator=(const DataWarehouse&) = delete;
	static DataWarehouse instance;
	bool HasImported = false;

	//小功能组
private:
	std::string SaveAdd();
	std::string SaveChange();
	std::string SaveType();
	std::string SavePrimaryKey();
	std::string IsDistinct();
	std::string IsIgnore();
	std::string HelpUpdate();
	std::string HelpWhere();
	std::string HelpGroupBy();
	std::string HelpHaving(const std::unordered_set<std::string>& Group_Set);
	std::string HelpOrder();
	std::string HelpLimit();

private:
	std::string HelpHelpWhereHaving(const int& x, const int& i);
	std::string HelpColumn();
	std::string HelpHelpColumn0(const int& row);
	std::string HelpHelpColumn1(const int& row);
	std::string HelpHelpColumn2(const int& row);
	std::string HelpHelpColumn3(const int& row);
	std::string HelpHelpColumn4(const int& row);
	std::string HelpHelpColumn10(const int& row);

public:
	MYSQL* GetConn();
	//一堆字段
	std::vector<std::string>& GetColumnName();
	std::vector<std::string>& GetColumnType();
	std::vector<int>& GetColumnB();
	std::vector<bool>& GetColumnIsPrimaryKey();
	std::vector<bool>& GetColumnIsNotNull();
	std::vector<bool>& GetColumnIsUnique();
	std::vector<bool>& GetColumnIsAuto_increasement();
	//用于插入数据的
	std::vector<std::string>& GetTitle();
	std::vector<std::vector<std::string>>& GetValue();
	//用于展示数据的
	std::vector<std::vector<std::string>>& GetColumn();
	std::vector<std::string>& GetAs();
	std::vector<std::string>& GetOrderByColumn();
	std::vector<bool>& GetOrderByIsAsc();
	std::vector<std::vector<std::string>>& GetTarColumn();
	std::vector<std::vector<std::string>>& GetCondition();
	std::vector<std::vector<std::string>>& GetOP();
	std::vector<std::vector<bool>>& GetConnecter();
	std::vector<std::string>& GetGroupBy();
	std::vector<int>& GetLimit();

	//数据库层次
public:
	//表示使用某个数据库
	void UseTest(const std::string& TarTable);
	//表示展示创建数据库的语句
	void ShowCreateDataBaseSql(const std::string& TarTable);
	//表示展示所有数据库
	void ShowDataBases();
	//表示展示某个数据库
	void ShowDataBase(const std::string& TarDataBase);
	//表示创建数据库
	void CreateDataBase(const std::string& TarTable);
	//表示删除某个数据库
	void DropDataBase(const std::string& TarTable);

	//数据表层次
public:
	//表示创建表
	void CreateTable(const std::string& TarTable);
	//查询所有表
	void ShowTable();
	//查询表格结构
	void DescTable(const std::string& TarTable);
	//删除数据表
	void DropTable(const std::string& TarTable);
	//修改表名
	void AlterTableName(const std::string& TarTable, const std::string& NewTableName);
	//添加字段
	void AddTableColumn(const std::string& TarTable);
	//修改字段
	void AlterTableColumn(const std::string& TarTable);
	//只修改字段类型
	void AlterTableColumnType(const std::string& TarTable);
	//创建表后添加主键约束
	void AlterAddPrimaryKey(const std::string& TarTable);
	//删除数据表主键约束
	void AlterDeletePrimaryKey(const std::string& TarTable);
	//插入元组
	void InsertRowData(const std::string& TarTable);
	//删除元组
	void DeleteRowData(const std::string& TarTable);
	//修改元组
	void UpdateRowData(const std::string& TarTable);
	//展示元组
	void ShowRowData(const std::string& TarTable);
	//决定重复
	DataWarehouse& ShowRowDataDistinct();
	//决定忽视
	DataWarehouse& InsertRowDataIgnore();

public:
	static DataWarehouse& getInstance();
	//是否有数据
	bool IsHaveData();
	bool IsMysqlAvailable()const;
	// 获取查询字段名
	std::vector<std::string> GetFieldNames();
	// 获取所有结果行数据
	std::vector<std::vector<std::string>> GetResultRows();
};