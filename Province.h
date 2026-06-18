#pragma once

class City;
class County;
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

class Province : public std::enable_shared_from_this<Province> ,public Goverment{

private:
	int ProvinceMapSize = 0;
	DataWarehouse& DataBase_;
	std::vector<std::shared_ptr<City>>ProvinceCityGroup;
	std::vector<std::shared_ptr<County>>ProvinceCountyGroup;

public:
	Province();
	Province(const std::wstring& FilePath, const bool& IsLoad);
	std::shared_ptr<City> FindCity(const std::string& name)const;
	std::shared_ptr<County> FindCounty(const std::string& name)const;
	int GetProvinceMapSize()const;
	std::vector<std::shared_ptr<City>>& GetCityGroup();
	std::vector<std::shared_ptr<County>>& GetCountyGroup();
	void JoinAllCity();
	void JoinAllCounty();

	void JoinPlayer(const std::shared_ptr<Player>& player);
	void fromjson(const nlohmann::json& j);
	nlohmann::json tojson()const;
};
