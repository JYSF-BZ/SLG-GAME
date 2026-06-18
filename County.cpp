#include "County.h"
#include"Province.h"
#include"City.h"
#include"Warehouse.h"
#include"Grid.h"
#include"Player.h"

County::County() :
	Goverment(),
	DataBase_(DataWarehouse::getInstance())
{
	ProvinceName = "";
}

County::County(const std::wstring& FilePath, const bool& IsLoad) :
	Goverment(),
	DataBase_(DataWarehouse::getInstance())
{
	std::ifstream file(FilePath);
	file.imbue(std::locale::classic());
	if (!file.is_open()) {
		std::wcout << FilePath << std::endl;
		std::cout << "打开文件失败" << std::endl;
		return;
	}
	std::string line;
	bool ans = 0;
	int row = -1;
	if (!IsLoad) {
		DataBase_.GetValue().push_back({});
		row = DataBase_.GetValue().size() - 1;
		DataBase_.GetValue()[row].resize(DataBase_.GetTitle().size());
	}
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		if (line == "结束") {
			break;
		}
		std::istringstream lineStream(line);
		if (ans == 0) {
			lineStream >> ID >> Name >> ProvinceName >> Site;
			if (!IsLoad) {
				DataBase_.GetValue()[row][0] = std::to_string(ID / 100);
				DataBase_.GetValue()[row][1] = ProvinceName;
				DataBase_.GetValue()[row][2] = Name;
			}
			ans = 1;
		}
		else {
			std::string temname;
			lineStream >> temname;
			WareHouse& warehouse = WareHouse::getInstance();
			std::vector<std::shared_ptr<City>>& temcitygroup = warehouse.GetCityGroup();
			int k = -1;
			for (int i = 0; i < temcitygroup.size(); i++) {
				if (temcitygroup[i]->GetName() == temname) {
					k = i;
					break;
				}
			}
			CountyMapSize += temcitygroup[k]->GetCityMap().size();
			if (temcitygroup[k]->GetOnlyMapSize().Hmin <OnlyMapSize.Hmin) {
				OnlyMapSize.Hmin = temcitygroup[k]->GetOnlyMapSize().Hmin;
			}
			if (temcitygroup[k]->GetOnlyMapSize().Hmax > OnlyMapSize.Hmax) {
				OnlyMapSize.Hmax = temcitygroup[k]->GetOnlyMapSize().Hmax;
			}
			if (temcitygroup[k]->GetOnlyMapSize().Wmin < OnlyMapSize.Wmin) {
				OnlyMapSize.Wmin = temcitygroup[k]->GetOnlyMapSize().Wmin;
			}
			if (temcitygroup[k]->GetOnlyMapSize().Wmax > OnlyMapSize.Wmax) {
				OnlyMapSize.Wmax = temcitygroup[k]->GetOnlyMapSize().Wmax;
			}
			CountyCityGroup.push_back(temcitygroup[k]);
		}
	}
	file.close();
}

int County::GetCountyMapSize()const {
	return CountyMapSize;
}

int County::Getjsonprovinceid()const {
	return jsonprovinceid;
}

void County::JoinProvince(const std::shared_ptr<Province>& province) {
	ProvinceBelong = province;
	jsonprovinceid = province->Getid();
}

void County::JsonJoinProvince(const std::shared_ptr<Province>& province){
	ProvinceBelong = province;
	province->GetCountyGroup().push_back(shared_from_this());
}

std::vector<std::shared_ptr<City>>& County::GetCityGroup() {
	return CountyCityGroup;
}

std::shared_ptr<Province> County::GetCountyProvince() const{
	return ProvinceBelong.lock();
}

std::shared_ptr<City> County::FindCity(const std::string& name)const {
	for (int i = 0; i < CountyCityGroup.size(); i++) {
		if (CountyCityGroup[i]->GetName() == name) {
			return CountyCityGroup[i];
		}
	}
	return nullptr;
}

void County::JoinAllCity() {
	for (auto& tarcity : CountyCityGroup) {
		tarcity->JoinCounty(shared_from_this());
	}
}

void County::JoinPlayer(const std::shared_ptr<Player>& player) {
	BelongPlayer = player;
	jsonplayerid = player->GetID();
	player->GetCountyGroup().push_back(shared_from_this());
}

void County::fromjson(const nlohmann::json& j) {
	ID = j["ID"];
	id = j["id"];
	OnlyMapSize.Hmin = j["MinCountyHigh"];
	OnlyMapSize.Hmax = j["MaxCountyHigh"];
	OnlyMapSize.Wmin = j["MinCountyWidth"];
	OnlyMapSize.Wmax = j["MaxCountyWidth"];
	CountyMapSize = j["CountyMapSize"];
	isBelong = j["isBelong"];
	Name = j["Name"];
	ProvinceName = j["ProvinceName"];
	jsonprovinceid = j["jsonprovinceid"];
	jsonplayerid = j["jsonplayerid"];
	Site = j["Site"];
}

nlohmann::json County::tojson()const {
	nlohmann::json j;
	j["ID"] = ID;
	j["id"] = id;
	j["MinCountyHigh"] = OnlyMapSize.Hmin;
	j["MaxCountyHigh"] = OnlyMapSize.Hmax;
	j["MinCountyWidth"] = OnlyMapSize.Wmin;
	j["MaxCountyWidth"] = OnlyMapSize.Wmax;
	j["CountyMapSize"] = CountyMapSize;
	j["isBelong"] = isBelong;
	j["Name"] = Name;
	j["ProvinceName"] = ProvinceName;
	j["jsonprovinceid"] = jsonprovinceid;
	j["jsonplayerid"] = jsonplayerid;
	j["Site"] = Site;
	return j;
}


