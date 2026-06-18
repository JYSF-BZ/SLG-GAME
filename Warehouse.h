#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<fstream>
#include<algorithm>
#include<string>
#include<vector>
#include<memory>
#include<filesystem>
#include"Grid.h"
#include"City.h"
#include"County.h"
#include"Province.h"
#include"Build.h"
#include"MySQL.h"
#include"Player.h"
#include"Chunk.h"

inline nlohmann::json tocolorjson(const sf::Color& c) {
	nlohmann::json j= nlohmann::json::array({ c.r, c.g, c.b, c.a });
	return j;
}

inline void fromcolorjson(const nlohmann::json& j, sf::Color& c) {
	c.r = j[0];
	c.g = j[1];
	c.b = j[2];
	c.a = j[3];
}

class WareHouse {
private:
	int HMIN = 0xFFFF, WMIN = 0xFFFF;
	int HMAX = 0, WMAX = 0;
	int CityGroupSize = 0, CountyGroupSize = 0, ProvinceGroupSize = 0, PlayerGroupSize = 0, BuildGroupSize = 0, ColorGroupSize = 0;
	bool IsExistDataBase = 0;
	std::vector<Chunk> MyChunk;
	std::vector<Chunk> MyCChunk;
	std::vector<sf::Font> FontGroup;
	std::vector<sf::Texture> TextureGroup;
	std::vector<sf::Sprite> SpriteGroup;
	std::vector<std::vector<sf::Texture>>LandMod;
	std::vector<std::vector<std::shared_ptr<Grid>>> Map;
	std::vector<std::shared_ptr<City>> CityGroup;
	std::vector<std::shared_ptr<County>> CountyGroup;
	std::vector<std::shared_ptr<Province>>ProvinceGroup;
	std::vector<Build>BuildGroup;
	std::vector<std::shared_ptr<Player>>PlayerGroup;
	std::vector<sf::Color>ColorGroup;
	std::vector<bool>IsColorGroup;
	DataWarehouse& DataBase_;

	void CutPicture(const sf::Texture& tartexture, int num, int site);
	std::wstring utf8_to_wstring(const std::string& utf8)const;

	void InitDataBase();
	void InitFont();
	void InitTexture();
	void Inittexture(const std::wstring& FilePath);
	void InitSprite();
	void InitPicture(sf::Sprite& Tarsprite);
	void InitChunk();
	void InitIsColorGroup();

	void ADDLANDMOD();
	void ADDCITY();
	void ADDCOUNTY();
	void ADDPROVINCE();
	void ADDMAP();
	void ADDBUILD();
	void ADDCOLOR();
	void AddCity(const std::string& ProvinceName, const std::string& Name, int id);
	void AddCounty(const std::string& ProvinceName, const std::string& Name, int id);
	void AddProvince(const std::string& ProvinceName,int id);
	void AddMap();

	void LOAD();
	void SAVE();

	WareHouse();
	~WareHouse();
	WareHouse(const WareHouse&) = delete;
	WareHouse& operator=(const WareHouse&) = delete;
	static WareHouse instance;
	bool hasImported = false;

public:
	static WareHouse& getInstance();

    void SetHMIN(int x);
    void SetWMIN(int x);
    void SetHMAX(int x);
    void SetWMAX(int x);
    void SetCityGroupSize(int x);
    void SetCountyGroupSize(int x);
    void SetProvinceGroupSize(int x);
    void SetBuildGroupSize(int x);
    void SetColorGroupSize(int x);
    void SetPlayerGroupSize(int x);

	void importData();
	int GetHMIN()const;
	int GetWMIN()const;
	int GetHMAX()const;
	int GetWMAX()const;
    int GetCityGroupSize()const;
    int GetCountyGroupSize()const;
    int GetProvinceGroupSize()const;
    int GetBuildGroupSize()const;
    int GetColorGroupSize()const;
    int GetPlayerGroupSize()const;
	bool GetIsExistDataBase()const;
	std::vector<std::vector<sf::Texture>>& GetLandMod();
	std::vector<std::shared_ptr<City>>& GetCityGroup();
	std::vector<std::shared_ptr<County>>& GetCountyGroup();
	std::vector<std::shared_ptr<Province>>& GetProvinceGroup();
	std::vector<std::vector<std::shared_ptr<Grid>>>& GetMap();
	std::vector<Build>& GetBuildGroup();
	std::vector<sf::Font>& GetFontGroup();
	std::vector<sf::Color>& GetColorGroup();
	std::vector<bool>& GetIsColorGroup();
	std::vector<std::shared_ptr<Player>>& GetPlayerGroup();
	std::vector<sf::Sprite>& GetSpriteGroup();
	std::vector<Chunk>& GetChunkGroup();
	std::vector<Chunk>& GetCChunkGroup();
};

namespace nlohmann{
    // Grid 本体
    template<>
    struct adl_serializer<Grid>{
        static void to_json(json& j, const Grid& g){
            j = g.tojson();
        }
        static void from_json(const json& j, Grid& g){
            g.fromjson(j);
        }
    };
    // Grid智能指针
    template<>
    struct adl_serializer<std::shared_ptr<Grid>>{
        static void to_json(json& j, const std::shared_ptr<Grid>& g) {
            if (g) {
                j = *g;
            }
        }
        static void from_json(const json& j, std::shared_ptr<Grid>& g){
            g = std::make_shared<Grid>();
            g->fromjson(j);
        }
    };

    // City本体
    template<>
    struct adl_serializer<City>{
        static void to_json(json& j, const City& c){
            j = c.tojson();
        }
        static void from_json(const json& j, City& c){
            c.fromjson(j);
        }
    };
    template<>
    struct adl_serializer<std::shared_ptr<City>>{
        static void to_json(json& j, const std::shared_ptr<City>& c) {
            if (c) {
                j = *c;
            }
        }
        static void from_json(const json& j, std::shared_ptr<City>& c){
            c = std::make_shared<City>();
            c->fromjson(j);
        }
    };

    // County本体
    template<>
    struct adl_serializer<County>{
        static void to_json(json& j, const County& c){
            j = c.tojson();
        }
        static void from_json(const json& j, County& c){
            c.fromjson(j);
        }
    };
    template<>
    struct adl_serializer<std::shared_ptr<County>>{
        static void to_json(json& j, const std::shared_ptr<County>& c) {
            if (c) {
                j = *c;
            }
        }
        static void from_json(const json& j, std::shared_ptr<County>& c){
            c = std::make_shared<County>();
            c->fromjson(j);
        }
    };

    // Province本体
    template<>
    struct adl_serializer<Province>{
        static void to_json(json& j, const Province& p){
            j = p.tojson();
        }
        static void from_json(const json& j, Province& p){
            p.fromjson(j);
        }
    };
    template<>
    struct adl_serializer<std::shared_ptr<Province>>{
        static void to_json(json& j, const std::shared_ptr<Province>& p) {
            if (p) {
                j = *p;
            }
        }
        static void from_json(const json& j, std::shared_ptr<Province>& p){
            p = std::make_shared<Province>();
            p->fromjson(j);
        }
    };

    // Player本体
    template<>
    struct adl_serializer<Player>{
        static void to_json(json& j, const Player& p){
            j = p.tojson();
        }
        static void from_json(const json& j, Player& p){
            p.fromjson(j);
        }
    };
    template<>
    struct adl_serializer<std::shared_ptr<Player>>{
        static void to_json(json& j, const std::shared_ptr<Player>& p) {
            if (p) {
                j = *p;
            }
        }
        static void from_json(const json& j, std::shared_ptr<Player>& p){
            p = std::make_shared<Player>();
            p->fromjson(j);
        }
    };

    // Build 无智能指针，只写本体
    template<>
    struct adl_serializer<Build>{
        static void to_json(json& j, const Build& b){
            j = b.tojson();
        }
        static void from_json(const json& j, Build& b){
            b.fromjson(j);
        }
    };
}
