#include "Province.h"
#include"County.h"
#include"City.h"
#include"Warehouse.h"
#include"Grid.h"
#include"Player.h"

Province::Province() :
	Goverment(),
	DataBase_(DataWarehouse::getInstance())
{


}

Province::Province(const std::wstring& FilePath, const bool& IsLoad) :
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
			lineStream >> ID >> Name >> Site;
			if (!IsLoad) {
				DataBase_.GetValue()[row][0] = std::to_string(ID / 10000);
				DataBase_.GetValue()[row][1] = Name;
			}
			ans = 1;
		}
		else {
			std::string temname;
			lineStream >> temname;
			WareHouse& warehouse = WareHouse::getInstance();
			std::vector<std::shared_ptr<County>>& temcountygroup = warehouse.GetCountyGroup();
			int k = -1;
			for (int i = 0; i < temcountygroup.size(); i++) {
				if (temcountygroup[i]->GetName() == temname) {
					k = i;
					break;
				}
			}
			if (temcountygroup[k]->GetOnlyMapSize().Hmin< OnlyMapSize.Hmin) {
				OnlyMapSize.Hmin = temcountygroup[k]->GetOnlyMapSize().Hmin;
			}
			if (temcountygroup[k]->GetOnlyMapSize().Hmax > OnlyMapSize.Hmax) {
				OnlyMapSize.Hmax = temcountygroup[k]->GetOnlyMapSize().Hmax;
			}
			if (temcountygroup[k]->GetOnlyMapSize().Wmin < OnlyMapSize.Wmin) {
				OnlyMapSize.Wmin = temcountygroup[k]->GetOnlyMapSize().Wmin;
			}
			if (temcountygroup[k]->GetOnlyMapSize().Wmax > OnlyMapSize.Wmax) {
				OnlyMapSize.Wmax = temcountygroup[k]->GetOnlyMapSize().Wmax;
			}
			for (auto& tar : temcountygroup[k]->GetCityGroup()) {
				ProvinceCityGroup.push_back(tar);
			}
			ProvinceCountyGroup.push_back(temcountygroup[k]);
		}
	}
	file.close();
}

std::shared_ptr<City> Province::FindCity(const std::string& name)const {
	for (int i = 0; i < ProvinceCityGroup.size(); i++) {
		if (ProvinceCityGroup[i]->GetName() == name) {
			return ProvinceCityGroup[i];
		}
	}
	return nullptr;
}

std::shared_ptr<County> Province::FindCounty(const std::string& name)const {
	for (int i = 0; i < ProvinceCountyGroup.size(); i++) {
		if (ProvinceCountyGroup[i]->GetName() == name) {
			return ProvinceCountyGroup[i];
		}
	}
	return nullptr;
}

int Province::GetProvinceMapSize()const {
	return ProvinceMapSize;
}

std::vector<std::shared_ptr<City>>& Province::GetCityGroup() {
	return ProvinceCityGroup;
}

std::vector<std::shared_ptr<County>>& Province::GetCountyGroup() {
	return ProvinceCountyGroup;
}

void Province::JoinAllCity() {
	for (auto& tarcity : ProvinceCityGroup) {
		tarcity->JoinProvince(shared_from_this());
	}
}

void Province::JoinAllCounty() {
	for (auto& tarcounty : ProvinceCountyGroup) {
		tarcounty->JoinProvince(shared_from_this());
	}
}

void Province::JoinPlayer(const std::shared_ptr<Player>& player) {
	BelongPlayer = player;
	jsonplayerid = player->GetID();
	player->GetProvinceGroup().push_back(shared_from_this());
}

void Province::fromjson(const nlohmann::json& j) {
	ID = j["ID"];
	id = j["id"];
	OnlyMapSize.Hmin = j["MinProvinceHigh"];
	OnlyMapSize.Hmax = j["MaxProvinceHigh"];
	OnlyMapSize.Wmin = j["MinProvinceWidth"];
	OnlyMapSize.Wmax = j["MaxProvinceWidth"];
	ProvinceMapSize = j["ProvinceMapSize"];
	isBelong = j["isBelong"];
	Name = j["Name"];
	jsonplayerid = j["jsonplayerid"];
	Site = j["Site"];
}

nlohmann::json Province::tojson()const {
	nlohmann::json j;
	j["ID"] = ID;
	j["id"] = id;
	j["MinProvinceHigh"] =  OnlyMapSize.Hmin;
	j["MaxProvinceHigh"] =  OnlyMapSize.Hmax;
	j["MinProvinceWidth"] = OnlyMapSize.Wmin;
	j["MaxProvinceWidth"] = OnlyMapSize.Wmax;
	j["ProvinceMapSize"] = ProvinceMapSize;
	j["isBelong"] = isBelong;
	j["Name"] = Name;
	j["jsonplayerid"] = jsonplayerid;
	j["Site"] = Site;
	return j;
}
