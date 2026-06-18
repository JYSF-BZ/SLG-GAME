#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<stack>
#include<locale>
#include<windows.h>
#include<cmath>
#include<fstream>
#include<stdexcept>
#include<filesystem>
#include<mysql.h>
#include"Player.h"
#include"Warehouse.h"
#include"Grid.h"
#include"City.h"
#include"County.h"
#include"Province.h"
#include"Build.h"
#include"MySQL.h"
#include"Chunk.h"

class Application {
private:
	//         pd               pd1                 pd2                pd3               pd4               pd5      
	//0      开始背景   备注：单指详细地图  备注：表示省级编号  备注：表示市级编号  备注：表示县级编号   各种事情
	//1      主背景           全国按钮         
	//2      详细地图       表示进入省级
	//3      主要地图       表示进入市级
	//4      查找地图       表示进入县级
	//5      查找玩家                                                                                      
	//6      创建玩家
	int pd = 0, pd1 = -1, pd2 = -1, pd3 = -1, pd4 = -1, pd5 = -1;
	int TarID, HMIN, WMIN, HMAX, WMAX;
	int PreX = 99, PreY = 210;
	int start = 0, len = 16, ChartMax = 0, tarselectbuttonx = -1, tarselectbuttony = -1;
	std::string ChartName = "", OrderByColumn = "";
	const int PreXB = 99, PreYB = 210, PYB = 30, PreStart = 0;
	int PY = 30, PreLen = 0;
	bool IsDraging = 0, IsAsc = 0, OrderByIsAsc = false;
	DataWarehouse& DataBase_;
	WareHouse& WareHouse_;
	std::vector<sf::Clock>ResourceGainClockGroup;
	std::vector<sf::Clock>TaxClockGroup;
	sf::Clock MouseClock;
	sf::Clock DialogClock;
	sf::Vector2f StartPosition = { 0,0 };
	std::wstring TarDialog = L"";
	sf::RenderWindow window_;
	sf::Font AllFont_;
	sf::Text TextEx_;
	sf::Text Dialog_;
	std::vector<std::shared_ptr<Grid>>TarMap;
	std::shared_ptr<Grid> temtarmap_;
	std::shared_ptr<Player> TarPlayer;
	std::vector<int> ChunkID;


	//拿一堆按钮
	std::vector<sf::Sprite>ButtonGroup_sprite_;
	std::vector<sf::Text>ButtonGroup_;
	std::vector<sf::Sprite>ProvinceButtonGroup_sprite_;
	std::vector<sf::Text>ProvinceButtonGroup_;
	std::vector<sf::Sprite>CountyButtonGroup_sprite_;
	std::vector<sf::Text>CountyButtonGroup_;
	std::vector<sf::Sprite>CityButtonGroup_sprite_;
	std::vector<sf::Text>CityButtonGroup_;
	std::vector<sf::Sprite>LittleButton_sprite_;
	std::vector<sf::Text>LittleButton_;
	std::vector<sf::Sprite>UseButton_sprite_;
	std::vector<sf::Text>UseButton_;
	std::vector<sf::Sprite>DisplayButton_sprite_;
	std::vector<sf::Text>DisplayButton_;
	std::vector<sf::Sprite>MainPdButton_sprite_;
	std::vector<sf::Text>MainPdButton_;
	std::vector<sf::Sprite>ShowDataGroupButton_sprite_;
	std::vector<sf::Text>ShowDataGroupButton_;
	std::vector<sf::Sprite>DataGroupButton_sprite_;
	std::vector<sf::Text>DataGroupButton_;

	//一堆临时用的按钮
	std::vector<sf::Sprite>ThingTypeButton_sprite_;
	std::vector<sf::Text>ThingTypeButton_;
	std::vector<sf::Sprite>BuildGroupButton_sprite_;
	std::vector<sf::Text>BuildGroupButton_;
	std::vector<sf::Sprite>PlayerGroupButton_sprite_;
	std::vector<sf::Text>PlayerGroupButton_;
	std::vector<sf::Sprite>ResourceButton_sprite_;
	std::vector<sf::Text>ResourceButton_;
	std::vector<sf::Sprite>LoseResourchButton_sprite_;
	std::vector<sf::Text>LoseResourchButton_;

	//导入县级城市库
	std::vector<std::shared_ptr<City>>& CityGroup_;
	//导入市级城市库
	std::vector<std::shared_ptr<County>>& CountyGroup_;
	//导入省级城市库
	std::vector<std::shared_ptr<Province>>& ProvinceGroup_;
	//导入总地图
	std::vector<std::vector<std::shared_ptr<Grid>>>& Map_;
	//弄一个总玩家集合
	std::vector<std::shared_ptr<Player>>& PlayerGroup_;
	//弄一个总颜色库
	std::vector<sf::Color> ColorGroup_;
	std::vector<bool>& IsColorGroup_;
	//导入建筑物集库
	std::vector<Build> BuildGroup_;
	//导入事件类型集库
	std::vector<std::string>ThingTypeGroup_;
	//导入土地
	std::vector<std::vector<sf::Texture>> LandMod;
	//导入精灵集
	std::vector<sf::Sprite> SpriteGroup_;
	//导入Chunk集
	std::vector<Chunk>& ChunkGroup_;
	std::vector<Chunk>& CChunkGroup_;

	//负责初始化和各种转换等工具
private:
	void INIT();
	void pixel_to_grid(const sf::Vector2f& temposition, int& x, int& y)const;
	std::wstring utf8_to_wstring(const std::string& utf8)const;
	std::string wstring_to_utf8(const std::wstring& wstr)const;
	sf::String utf8_to_String(const std::string& str)const;
	std::string String_to_utf8(const sf::String& str)const;
	int GetCharCount(const std::wstring& str)const;
	bool CheckResourceNum(const Build& tarbuild,const std::shared_ptr<Player>& tarplayer)const;
	void SetTextSize(sf::Text& tartext, const sf::Sprite& tarsprite, float padding = 8.0f);
	bool IsPressButton(const sf::Sprite& tarsprite, const sf::Event::MouseButtonPressed& mouse)const;

	//主要运行
public:
	Application();
	bool IsRunning()const;
	void ProcessEvents();
	void Update();
	void Render();

	//获取失去郡县函数
private:
	void AddCity(const std::shared_ptr<Player>& player, const std::shared_ptr<City>& city)const;
	void EraseCity(const std::shared_ptr<Player>& player,const std::shared_ptr<City>& city)const;
	void AddCounty(const std::shared_ptr<Player>& player,const std::shared_ptr<County>& county)const;
	void EraseCounty(const std::shared_ptr<Player>& player,const std::shared_ptr<County>& county)const;
	void AddProvince(const std::shared_ptr<Player>& player,const std::shared_ptr<Province>& province)const;
	void EraseProvince(const std::shared_ptr<Player>& player,const std::shared_ptr<Province>& province)const;

	//负责构造地图图像
private:
	void UpdateMap();
	void UpdateMap(int ID);
	void UpdateMap(int XStart, int XEnd, int YStart,int YEnd);
	void Change(const std::shared_ptr<Grid>& onlytarmap);

	//负责管理郡县
private:
	std::shared_ptr<Grid> GainGrid(int tarx,int tary)const;
	std::shared_ptr<City> GainCity(int tarID)const;
	std::shared_ptr<City> GainCity(const std::string& tarname)const;
	std::shared_ptr<County> GainCounty(int tarID)const;
	std::shared_ptr<County> GainCounty(const std::string& tarname)const;
	std::shared_ptr<Province> GainProvince(int tarID)const;
	std::shared_ptr<Province> GainProvince(const std::string& tarname)const;
	std::shared_ptr<Player> GainPlayer(const std::string& tarname)const;

	//初始化库
private:
	void InitThingTypeGroup();
	void InitBorder();
	void InitShowDataGroupTableGroup();
	void InitDataGroup();

	//初始化一堆东西
private:
	void InitText(sf::Text& tartext)const;
	void InitPicture(sf::Sprite& Tarsprite)const;
	void InitOriButton();
	void InitStartGameButton();
	void InitButton();
	void InitButton(const std::shared_ptr<Grid>& onlytarmap);
	void InitButton(const std::wstring& FilePath, std::vector<sf::Text>& tarbuttongroup, std::vector<sf::Sprite>& tarbuttonggroup_sprite)const;
	void InitButton(std::vector<sf::Text>& tarbuttongroup, std::vector<sf::Sprite>& tarbuttonggroup_sprite);
	void InitButton(int ID);
	void InitDisplayButton();
	void InitButtonFindPlayer(const std::shared_ptr<Player>& player);
	void InitChooseColorButton(const std::shared_ptr<Player>& player);
	void InitDialog(const std::string& Str);
	void InitNum();
	template<typename T>
	void InitDisplayButton(const std::shared_ptr<Player>& player, std::vector<T>& targroup);

	//开始后的初始化
	void InitChooseThingTypeButton(int startx, int endx, int starty,int endy);
	void InitBuildGroupButton(int startx, int endx, int starty, int endy);
	void InitPlayerGroupButton(int startx,int endx, int starty, int endy);
	void InitResourceButton(int startx, int endx, int starty, int endy);
	void InitLoseDisplayButton(int startx, int endx, int starty, int endy, const Build& tembuild);

	//画出来
private:
	void DRAW(const std::vector<std::shared_ptr<Grid>>& tarmap);
	void DRAW(std::vector<Grid>& tarmap);
	void DRAW(const sf::Sprite& tarsprite);
	void DRAW(int ID = 1000000, bool Ctype = 0);
	template<typename T>
	void DRAW(const std::vector<T>& tarSpriteGroup);
	template<typename T>
	void DRAW(const std::vector<std::vector<T>>& tarSpriteGroup);
	template<typename T>
	void DRAWWindow(const sf::Sprite& tarsprite,const std::vector<T>& targroup);

	//辅助函数
private:
	void HelpChangeMap();
	void HelpFindCity();
	void HelpFind();
	void HelpCreate();
	void HelpBackSpace();
	void HelpSetColor(int x);
	void HelpCountDataGroupNum();

	//事件类型辅助函数
	void HelpJoinGrid(const std::shared_ptr<Player>& tarplayer,const std::shared_ptr<Grid>& targrid);
	void HelpFightWin();
	void HelpFightLose();
	void HelpFightHelp();
	void HelpFight();
	void HelpBuildHotel();
	void HelpBuildBridge();
	void HelpMove();
	void HelpDemolish();
	void HelpTax();

	//事件辅助触发辅助分函数
private:
	void Help_Pd3_Pd13xx10(const sf::Event::MouseButtonPressed& mouse);

	//事件辅助触发函数
private:
	//主背景
	void HelpPd0(const sf::Event::MouseButtonPressed& mouse);
	//选择模块
	void HelpPd1(const sf::Event::MouseButtonPressed& mouse);
	//查看详细地图
	void HelpPd2(const sf::Event::MouseButtonPressed& mouse);
	//查看主要地图
	void HelpPd3(const sf::Event::MouseButtonPressed& mouse);
	//查看势力地图
	void HelpPd4(const sf::Event::MouseButtonPressed& mouse);
	//查看玩家明细
	void HelpPd5(const sf::Event::MouseButtonPressed& mouse);
	//创建玩家
	void HelpPd6(const sf::Event::MouseButtonPressed& mouse);
	//查询城池库
	void HelpPd7(const sf::Event::MouseButtonPressed& mouse);

	//事件触发
private:
	void HandleEvent(const sf::Event::Closed&);
	void HandleEvent(const sf::Event::Resized& resized);
	void HandleEvent(const sf::Event::FocusLost&);
	void HandleEvent(const sf::Event::FocusGained&);
	void HandleEvent(const sf::Event::KeyPressed& key);
	void HandleEvent(const sf::Event::KeyReleased& key);
	void HandleEvent(const sf::Event::MouseButtonPressed& mouse);
	void HandleEvent(const sf::Event::MouseButtonReleased& mouse);
	void HandleEvent(const sf::Event::MouseWheelScrolled& mouse);
	void HandleEvent(const sf::Event::MouseMoved& mouse);
	void HandleEvent(const sf::Event::TextEntered& textEvent);
	void HandleEvent(const auto&) {};

	//验证要用的
private:
	void TemAddCity();
	void TemAddCounty();
	void TemAddProvince();
};