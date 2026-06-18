#pragma once

class City;
class County;
class Province;
class Grid;

#include<iostream>
#include<string>
#include<vector>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<Random>
#include<ctime>
#include"Build.h"

class Player {
private:
	int jsoncolor = -1;
	int jsonbelongcity = -1;
	int jsoncapital = -1;

private:
	int ID;
	std::string Name;
	bool Live;
	sf::Color color;
	std::shared_ptr<City> BelongCity;
	std::shared_ptr<City> Capital;
	std::vector<std::shared_ptr<City>>CityGroup;
	std::vector<std::shared_ptr<County>>CountyGroup;
	std::vector<std::shared_ptr<Province>>ProvinceGroup;
	std::vector<std::shared_ptr<Grid>>GridGroup;

private:
	int GrainRemain, IronRemain, TimberRemain, StoneRemain, MoneyRemain;
	int GrainGain, IronGain, TimberGain, StoneGain, MoneyGain;

public:
	Player();
	Player(const std::string& name, int id);
	void SetBelongCity(const std::shared_ptr<City>& city);
	void SetColor(const sf::Color& tarcolor, int colorid);
	void SetLive();
	void SetCapital(const std::shared_ptr<City>& city);
	int GetID()const;
	int Getjsoncolor()const;
	int Getjsonbelongcity()const;
	int Getjsoncapital()const;
	bool IsLive()const;
	bool IsCapital()const;
	const std::string& GetName()const;
	std::shared_ptr<City>& GetBelongCity();
	std::shared_ptr<City>& GetCapital();
	sf::Color& GetColor();
	std::vector<std::shared_ptr<City>>& GetCityGroup();
	std::vector<std::shared_ptr<County>>& GetCountyGroup();
	std::vector<std::shared_ptr<Province>>& GetProvinceGroup();
	std::vector<std::shared_ptr<Grid>>& GetGridGroup();

public:
	void AddGainResource(const std::shared_ptr<Grid>& targrid);
	void EraseGainResource(const std::shared_ptr<Grid>& targrid);
	void AddRemainResource();
	void AddRemainMoneyResource();
	void EraseRemainResource(const std::string& type);
	void fromjson(const nlohmann::json& j);
	nlohmann::json tojson()const;
	int GetResourceRemain(const std::string& resourcetype)const;
	int GetResourceGain(const std::string& resourcetype)const;
};
