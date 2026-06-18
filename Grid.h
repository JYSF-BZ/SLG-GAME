#pragma once

class City;
class County;
class Province;
class Player;
class WareHouse;

#include<string>
#include<stack>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<vector>

class Grid : public std::enable_shared_from_this<Grid>, public sf::Drawable {
	//存档
private:
	int jsoncityid = -1;
	int jsonplayerid = -1;
	bool jsonlittlemap = 0;
	std::vector<int> jsonimagenum;

private:
	sf::RectangleShape rectangle_;
	sf::Texture texture_;
	std::stack<sf::Sprite> sprite_;
	std::string site;
	std::string temsite;
	int resourcenum;
	int x, y;
	bool cityup = false, citydown = false, cityleft = false, cityright = false, countyup = false, countydown = false, countyleft = false, countyright = false;
	bool issprite;
	bool isnull = 0;
	std::weak_ptr<City> BelongCity;
	float NowX, NowY;
	float NowSizeX, NowSizeY;
	std::weak_ptr<Player> BelongPlayer;
	bool IsInMap = 1;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::string GetLastTowChar(const std::string& str, int n = 2)const;

public:
	Grid();
	Grid(int tarx, int tary);
	sf::RectangleShape& GetGrid();
	std::shared_ptr<City> GetBelongCity()const;
	std::shared_ptr<County> GetBelongCounty()const;
	std::shared_ptr<Province> GetBelongProvince()const;
	std::shared_ptr<Player> GetBelongPlayer()const;
	std::vector<int>& Getjsonimagenum();
	void SetUp(bool IsCity = 1);
	void SetDown(bool IsCity = 1);
	void SetRight(bool IsCity = 1);
	void SetLeft(bool IsCity = 1);
	void JoinCity(const std::shared_ptr<City>& city);
	void SetNowPosition(float x, float y);
	void SetNowSpritePosition(float x, float y);
	void SetNowSize(float x, float y);
	void SetNowSpriteSize(float x, float y);
	void SetSite(const std::string& Site);
	void SetPlayer(const std::shared_ptr<Player>& player);
	void SetSprite(const sf::Texture& tex);
	void SetSprite1(const sf::Texture& tex);
	void SetTemsite(const std::string& tarsite);
	void SetIsMap();
	void SetNull(int tary, int x);
	void RemoveSprite();
	void SetResource(int levelx = 10);
	void Setjsonlittlemap();
	void JsonJoinPlayer(std::shared_ptr<Player> player);
	void JsonJoinCity(std::shared_ptr<City> city);
	void fromjson(const nlohmann::json& j);
	nlohmann::json tojson()const;
	std::string GetSite()const;
	std::string GetType()const;
	int GetResouceNum()const;
	int GetX()const;
	int GetY()const;
	int Getjsoncityid()const;
	int Getjsonplayerid()const;
	float GetNowX()const;
	float GetNowY()const;
	float GetNowSizeX()const;
	float GetNowSizeY()const;
	bool IsMap()const;
	bool IsSprite()const;
	bool IsNull()const;
	bool isjsonlittlemap()const;
	bool IsUp(bool IsCity = 1) const;
	bool IsDown(bool IsCity = 1) const;
	bool IsRight(bool IsCity = 1) const;
	bool IsLeft(bool IsCity = 1) const;
	sf::Sprite& GetSprite();
};