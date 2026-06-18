#pragma once

class County;
class Province;
class Grid;
class Player;

#include<iostream>
#include<fstream>
#include<string>
#include<nlohmann/json.hpp>
#include<vector>
#include<unordered_set>
#include"Player.h"
#include"MySQL.h"
#include"Goverment.h"

class City : public std::enable_shared_from_this<City>,public Goverment {
private:
	int jsoncountyid = -1;
	int jsonprovinceid = -1;

private:
	int Level;
	int MainX, MainY;
	std::string CountyName;
	std::string ProvinceName;
	DataWarehouse& DataBase_;
	std::weak_ptr<County> CountyBelong;
	std::weak_ptr<Province> ProvinceBelong;
	std::vector<std::shared_ptr<Grid>>CityMap;
	std::vector<Grid>CityMaps;
	std::vector<std::shared_ptr<Grid>>LittleMap;

public:
	City();
	City(const std::wstring& FilePath, const bool& IsLoad);
	int Getjsoncountyid()const;
	int Getjsonprovinceid()const;
	std::vector<std::shared_ptr<Grid>>& GetCityMap();
	std::vector<std::shared_ptr<Grid>>& GetLittleMap();
	std::vector<Grid>& GetCityMaps();
	std::shared_ptr<County> GetCityCounty()const;
	std::shared_ptr<Province> GetCityProvince()const;
	void JoinCounty(const std::shared_ptr<County>& county);
	void JsonJoinCounty(const std::shared_ptr<County>& county);
	void JoinProvince(const std::shared_ptr<Province>& province);
	void JsonJoinProvince(const std::shared_ptr<Province>& province);
	void SetMainPosition(int x, int y);
	void JoinAllGrid();
	int GetMainX()const;
	int GetMainY()const;
	int GetLevel()const;

	void JoinPlayer(const std::shared_ptr<Player>& player) override;
	void fromjson(const nlohmann::json& j) override;
	nlohmann::json tojson()const override;
};

