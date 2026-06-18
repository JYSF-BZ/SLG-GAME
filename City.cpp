#include "City.h"
#include"County.h"
#include"Goverment.h"
#include"Grid.h"
#include"Province.h"
#include"Player.h"
#include"Warehouse.h"

City::City() :
	Goverment(),
	DataBase_(DataWarehouse::getInstance())
{
	CountyName = "";
	ProvinceName = "";
	Level = 0;
}

City::City(const std::wstring& FilePath, const bool& IsLoad) :
	Goverment(),
	DataBase_(DataWarehouse::getInstance())
{
	LittleMap.reserve(121);
	isBelong = 0;
	std::ifstream file(FilePath);
	file.imbue(std::locale::classic());
	if (!file.is_open()) {
		std::wcout << FilePath << std::endl;
		std::cout << "打开文件失败" << std::endl;

		return;
	}
	std::string line;
	CityMap.clear();
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
		std::istringstream lineStream(line);
		if (ans == 0) {
			lineStream >> ID >> Name >> CountyName >> ProvinceName >> Site >> Level;
			if (!IsLoad) {
				DataBase_.GetValue()[row][0] = std::to_string(ID);
				DataBase_.GetValue()[row][1] = ProvinceName;
				DataBase_.GetValue()[row][2] = CountyName;
				DataBase_.GetValue()[row][3] = Name;
				DataBase_.GetValue()[row][4] = Site;
				DataBase_.GetValue()[row][5] = "(0,0)";
				DataBase_.GetValue()[row][6] = std::to_string(Level);
			}
			ans = 1;
		}
		else {
			int Tall, InitWidth, EndWidth;
			lineStream >> Tall >> InitWidth >> EndWidth;
			if (OnlyMapSize.Hmin > Tall) {
				OnlyMapSize.Hmin = Tall;
			}
			if (OnlyMapSize.Hmax < Tall) {
				OnlyMapSize.Hmax = Tall;
			}
			if (OnlyMapSize.Wmin > InitWidth) {
				OnlyMapSize.Wmin = InitWidth;
			}
			if (OnlyMapSize.Wmax < EndWidth) {
				OnlyMapSize.Wmax = EndWidth;
			}
			for (int i = InitWidth; i <= EndWidth; i++) {
				int color;
				lineStream >> color;
				auto temrectangle = std::make_shared<Grid>(i, Tall);
				int type, r, g, b;
				int baseR, baseG, baseB;
				type = rand() % 3;
				if (color == 1) {
					//农田
					temrectangle->SetSite("农田");
				}
				else if (color == 2) {
					//河流
					temrectangle->SetSite("河流");
				}
				else if (color == 3) {
					//山脉
					temrectangle->SetSite("山脉");
				}
				else if (color == 4) {
					//城墙
					temrectangle->SetSite("城墙");
					temrectangle->Setjsonlittlemap();
					LittleMap.push_back(temrectangle);
				}
				else if (color == 5) {
					//深山
					temrectangle->SetSite("深山");
				}
				else if (color == 6) {
					//市井
					temrectangle->SetSite("市井");
					temrectangle->Setjsonlittlemap();
					LittleMap.push_back(temrectangle);
				}
				else if (color == 7) {
					//政府
					temrectangle->SetSite("政府");
					SetMainPosition(i, Tall);
					temrectangle->Setjsonlittlemap();
					if (!IsLoad) {
						DataBase_.GetValue()[row][5] = "(" + std::to_string(Tall) + "," + std::to_string(i) + ")";
					}
					LittleMap.push_back(temrectangle);
				}
				else if (color == 9) {
					temrectangle->GetGrid().setFillColor(sf::Color::Red);
				}
				CityMap.push_back(temrectangle);
				CityMaps.push_back(*temrectangle);
			}
		}
	}
	file.close();
}

int City::Getjsoncountyid()const {
	return jsoncountyid;
}

int City::Getjsonprovinceid()const {
	return jsonprovinceid;
}

std::vector<std::shared_ptr<Grid>>& City::GetCityMap() {
	return CityMap;
}

std::vector<std::shared_ptr<Grid>>& City::GetLittleMap() {
	return LittleMap;
}

std::vector<Grid>& City::GetCityMaps() {
	return CityMaps;
}

std::shared_ptr<County> City::GetCityCounty()const {
	return CountyBelong.lock();
}

std::shared_ptr<Province> City::GetCityProvince()const {
	return ProvinceBelong.lock();
}

void City::JoinCounty(const std::shared_ptr<County>& county) {
	CountyBelong = county;
	jsoncountyid = county->Getid();
}

void City::JsonJoinCounty(const std::shared_ptr<County>& county){
	CountyBelong = county;
	county->GetCityGroup().push_back(shared_from_this());
}

void City::JoinProvince(const std::shared_ptr<Province>& province) {
	ProvinceBelong = province;
	jsonprovinceid = province->Getid();
}

void City::JsonJoinProvince(const std::shared_ptr<Province>& province){
	ProvinceBelong = province;
	province->GetCityGroup().push_back(shared_from_this());
}

void City::SetMainPosition(int x, int y) {
	MainX = x;
	MainY = y;
}

void City::JoinAllGrid() {
	for (auto& grid : CityMap) {
		grid->JoinCity(shared_from_this());
	}
}

int City::GetMainX()const {
	return MainX;
}

int City::GetMainY()const {
	return MainY;
}

int City::GetLevel()const {
	return Level;
}

void City::JoinPlayer(const std::shared_ptr<Player>& player) {
	BelongPlayer = player;
	jsonplayerid = player->GetID();
	player->GetCityGroup().push_back(shared_from_this());
}

void City::fromjson(const nlohmann::json& j) {
	OnlyMapSize.Hmin = j["MinCityHigh"];
	OnlyMapSize.Hmax = j["MaxCityHigh"];
	OnlyMapSize.Wmin = j["MinCityWidth"];
	OnlyMapSize.Wmax = j["MaxCityWidth"];
	ID = j["ID"];
	id = j["id"];
	Level = j["Level"];
	MainX = j["MainX"];
	MainY = j["MainY"];
	Name = j["Name"];
	CountyName = j["CountyName"];
	ProvinceName = j["ProvinceName"];
	Site = j["Site"];
	jsoncountyid = j["jsoncountyid"];
	jsonprovinceid = j["jsonprovinceid"];
	jsonplayerid = j["jsonplayerid"];
	isBelong = j["isBelong"];

}

nlohmann::json City::tojson()const {
	nlohmann::json j;

	j["MinCityHigh"] = OnlyMapSize.Hmin;
	j["MaxCityHigh"] = OnlyMapSize.Hmax;
	j["MinCityWidth"] = OnlyMapSize.Wmin;
	j["MaxCityWidth"] = OnlyMapSize.Wmax;
	j["ID"] = ID;
	j["id"] = id;
	j["Level"] = Level;
	j["MainX"] = MainX;
	j["MainY"] = MainY;
	j["Name"] = Name;
	j["CountyName"] = CountyName;
	j["ProvinceName"] = ProvinceName;
	j["Site"] = Site;
	j["isBelong"] = isBelong;
	j["jsoncountyid"] = jsoncountyid;
	j["jsonprovinceid"] = jsonprovinceid;
	j["jsonplayerid"] = jsonplayerid;

	return j;
}



