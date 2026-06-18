#include"Player.h"
#include"City.h"
#include"County.h"
#include"Province.h"
#include"Warehouse.h"
#include"Grid.h"

Player::Player() {
	GrainGain = 100;
	StoneGain = 100;
	IronGain = 100;
	TimberGain = 100;
	MoneyGain = 0;
	GrainRemain = 5000;
	StoneRemain = 5000;
	TimberRemain = 5000;
	IronRemain = 5000;
	MoneyRemain = 5000;
	Name = "";
	Live = 1;
	Capital = nullptr;
}

Player::Player(const std::string& name, int id) {
	GrainGain = 100;
	StoneGain = 100;
	IronGain = 100;
	TimberGain = 100;
	MoneyGain = 0;
	GrainRemain = 5000;
	StoneRemain = 5000;
	TimberRemain = 5000;
	IronRemain = 5000;
	MoneyRemain = 5000;
	ID = id;
	Name = name;
	Live = 1;
	Capital = nullptr;
}

void Player::SetBelongCity(const std::shared_ptr<City>& city) {
	std::cout << Name << "的设置归属城池函数执行开始-----------------------" << std::endl;
	BelongCity = city;
	jsonbelongcity = city->Getid();
	std::cout << Name << "的设置归属城池函数执行结束" << std::endl;
}

void Player::SetColor(const sf::Color& tarcolor, int colorid) {
	std::cout << "设置玩家颜色函数执行开始-----------------------" << std::endl;
	color = tarcolor;
	jsoncolor = colorid;
	std::cout << "设置玩家颜色函数执行结束" << std::endl;
}

void Player::SetLive() {
	std::cout << Name << "的死亡函数执行开始-----------------------" << std::endl;
	Live = 0;
	std::cout << Name << "的死亡函数执行结束" << std::endl;
}

void Player::SetCapital(const std::shared_ptr<City>& city) {
	Capital = city;
	jsoncapital = city->Getid();
}

int Player::GetID()const {
	return ID;
}

int Player::Getjsoncolor()const {
	return jsoncolor;
}

int Player::Getjsonbelongcity()const {
	return jsonbelongcity;
}

int Player::Getjsoncapital() const{
	return jsoncapital;
}

bool Player::IsLive()const {
	return Live;
}

bool Player::IsCapital()const {
	if (Capital == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

const std::string& Player::GetName()const {
	return Name;
}

std::shared_ptr<City>& Player::GetBelongCity() {
	return BelongCity;
}

std::shared_ptr<City>& Player::GetCapital() {
	return Capital;
}

sf::Color& Player::GetColor() {
	return color;
}

std::vector<std::shared_ptr<City>>& Player::GetCityGroup() {
	return CityGroup;
}

std::vector<std::shared_ptr<County>>& Player::GetCountyGroup() {
	return CountyGroup;
}

std::vector<std::shared_ptr<Province>>& Player::GetProvinceGroup() {
	return ProvinceGroup;
}

std::vector<std::shared_ptr<Grid>>& Player::GetGridGroup() {
	return GridGroup;
}

void Player::AddGainResource(const std::shared_ptr<Grid>& targrid) {
	std::cout << targrid->GetBelongPlayer()->GetName() << "的增加资源产量函数执行开始-----------------------" << std::endl;
	if (targrid->GetType() == "土地") {
		GrainGain += targrid->GetResouceNum();
		IronGain += targrid->GetResouceNum();
		TimberGain += targrid->GetResouceNum();
		StoneGain += targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "粮草") {
		GrainGain += targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "铁矿") {
		IronGain += targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "木材") {
		TimberGain += targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "石料") {
		StoneGain += targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "市井" || targrid->GetType() == "城墙") {
		MoneyGain += targrid->GetResouceNum();
	}
	std::cout << targrid->GetBelongPlayer()->GetName() << "的增加资源产量函数执行结束" << std::endl;
}

void Player::EraseGainResource(const std::shared_ptr<Grid>& targrid) {
	std::cout << targrid->GetBelongPlayer()->GetName() << "的减少资源产量函数执行开始-----------------------" << std::endl;
	if (targrid->GetType() == "土地") {
		GrainGain -= targrid->GetResouceNum();
		IronGain -= targrid->GetResouceNum();
		TimberGain -= targrid->GetResouceNum();
		StoneGain -= targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "粮草") {
		GrainGain -= targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "铁矿") {
		IronGain -= targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "木材") {
		TimberGain -= targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "石料") {
		StoneGain -= targrid->GetResouceNum();
	}
	else if (targrid->GetType() == "市井" || targrid->GetType() == "城墙") {
		MoneyGain -= targrid->GetResouceNum();
	}
	std::cout << targrid->GetBelongPlayer()->GetName() << "的减少资源产量函数执行结束" << std::endl;
}

void Player::AddRemainResource() {
	std::cout << Name << "的增加资源函数执行开始-----------------------" << std::endl;
	GrainRemain += GrainGain;
	IronRemain += IronGain;
	TimberRemain += TimberGain;
	StoneRemain += StoneGain;
	std::cout << Name << "的增加资源函数执行结束" << std::endl;
}

void Player::AddRemainMoneyResource() {
	std::cout << Name << "的增加铜币产量函数执行开始-----------------------" << std::endl;
	MoneyRemain += MoneyGain;
	std::cout << Name << "的增加铜币产量函数执行结束" << std::endl;
}

void Player::EraseRemainResource(const std::string& type) {
	std::cout << Name << "的减少资源函数执行开始-----------------------" << std::endl;
	WareHouse& warehouse = WareHouse::getInstance();
	std::vector<Build>& tembuildgroup = warehouse.GetBuildGroup();
	for (auto& tarbuild : tembuildgroup) {
		if (tarbuild.GetName() == type) {
			GrainRemain -= tarbuild.GetLoseResourch()[0];
			IronRemain -= tarbuild.GetLoseResourch()[1];
			TimberRemain -= tarbuild.GetLoseResourch()[2];
			StoneRemain -= tarbuild.GetLoseResourch()[3];
			MoneyRemain -= tarbuild.GetLoseResourch()[4];
			break;
		}
	}
	std::cout << Name << "的减少资源函数执行结束" << std::endl;
}

void Player::fromjson(const nlohmann::json& j) {
	ID = j["ID"];
	Name = j["Name"];
	Live = j["Live"];
	jsoncolor = j["jsoncolor"];
	jsonbelongcity = j["jsonbelongcity"];
	jsoncapital = j["jsoncapital"];
	GrainRemain = j["GrainRemain"];
	IronRemain = j["IronRemain"];
	TimberRemain = j["TimberRemain"];
	StoneRemain = j["StoneRemain"];
	MoneyRemain = j["MoneyRemain"];
	GrainGain = j["GrainGain"];
	IronGain = j["IronGain"];
	TimberGain = j["TimberGain"];
	StoneGain = j["StoneGain"];
	MoneyGain = j["MoneyGain"];
}

nlohmann::json Player::tojson()const {
	nlohmann::json j;
	j["ID"] = ID;
	j["Name"] = Name;
	j["Live"] = Live;
	j["jsoncolor"] = jsoncolor;
	j["jsonbelongcity"] = jsonbelongcity;
	j["jsoncapital"] = jsoncapital;
	j["GrainRemain"] = GrainRemain;
	j["IronRemain"] = IronRemain;
	j["TimberRemain"] = TimberRemain;
	j["StoneRemain"] = StoneRemain;
	j["MoneyRemain"] = MoneyRemain;
	j["GrainGain"] = GrainGain;
	j["IronGain"] = IronGain;
	j["TimberGain"] = TimberGain;
	j["StoneGain"] = StoneGain;
	j["MoneyGain"] = MoneyGain;
	return j;
}

int Player::GetResourceRemain(const std::string& resourcetype)const {
	if (resourcetype == "粮草") {
		return GrainRemain;
	}
	else if (resourcetype == "铁矿") {
		return IronRemain;
	}
	else if (resourcetype == "木材") {
		return TimberRemain;
	}
	else if (resourcetype == "石料") {
		return StoneRemain;
	}
	else if (resourcetype == "铜币") {
		return MoneyRemain;
	}
}

int Player::GetResourceGain(const std::string& resourcetype)const {
	if (resourcetype == "粮草") {
		return GrainGain;
	}
	else if (resourcetype == "铁矿") {
		return IronGain;
	}
	else if (resourcetype == "木材") {
		return TimberGain;
	}
	else if (resourcetype == "石料") {
		return StoneGain;
	}
	else if (resourcetype == "铜币") {
		return MoneyGain;
	}
}
