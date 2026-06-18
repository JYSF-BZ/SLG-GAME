#pragma once

class City;
class Province;
class WareHouse;
class Grid;
class Player;

#include<vector>
#include<string>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<sstream>
#include"Goverment.h"
#include"MySQL.h"

class County : public std::enable_shared_from_this<County>,public Goverment {
private:
	int jsonprovinceid = -1;

private:
	int CountyMapSize = 0;
	std::string ProvinceName;
	DataWarehouse& DataBase_;
	std::weak_ptr<Province> ProvinceBelong;
	std::vector<std::shared_ptr<City>>CountyCityGroup;

public:
	County();
	County(const std::wstring& FilePath, const bool& IsLoad);
	int GetCountyMapSize()const;
	int Getjsonprovinceid()const;
	void JoinProvince(const std::shared_ptr<Province>& province);
	void JsonJoinProvince(const std::shared_ptr<Province>& province);
	std::vector<std::shared_ptr<City>>& GetCityGroup();
	std::shared_ptr<Province> GetCountyProvince()const;
	std::shared_ptr<City> FindCity(const std::string& name)const;
	void JoinAllCity();

	void JoinPlayer(const std::shared_ptr<Player>& player);
	void fromjson(const nlohmann::json& j);
	nlohmann::json tojson()const;
};
