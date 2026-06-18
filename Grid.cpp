#include "Grid.h"
#include"City.h"
#include"County.h"
#include"Province.h"
#include"Player.h"
#include"WareHouse.h"

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!sprite_.empty()) {
		target.draw(sprite_.top(), states);
	}
}

std::string Grid::GetLastTowChar(const std::string& str, int n)const {
	if (str.empty() || n <= 0) {
		return "";
	}

	int count = 0;
	int pos = (int)str.size();

	// 从后往前遍历 UTF-8 字符
	for (int i = (int)str.size() - 1; i >= 0; --i) {
		uint8_t c = (uint8_t)str[i];

		// UTF-8 规则：以 0xxxxxxx 或 11xxxxxx 开头 = 字符的起始位置
		if ((c < 0x80) || (c >= 0xC0)) {
			count++;
			if (count == n) {
				pos = i;
				break;
			}
		}
	}

	return str.substr(pos);
}

Grid::Grid() {
	issprite = 0;
	x = 0;
	y = 0;
	rectangle_.setSize({ 1,1 });
	rectangle_.setFillColor(sf::Color::White);
	rectangle_.setOrigin({ 0,0 });
	rectangle_.setPosition({ 0.0,0.0 });
	BelongPlayer.reset();
	BelongCity.reset();
	resourcenum = 0;
	sprite_.emplace(texture_);
	temsite = "";
}

Grid::Grid(int tarx, int tary) {
	issprite = 0;
	BelongPlayer.reset();
	BelongCity.reset();
	x = tarx;
	y = tary;
	rectangle_.setSize({ 1,1 });
	rectangle_.setFillColor(sf::Color::White);
	rectangle_.setOrigin({ 0,0 });
	rectangle_.setPosition({ static_cast<float>(x),static_cast<float>(y) });
	resourcenum = 0;
	sprite_.emplace(texture_);
	temsite = "";
}

sf::RectangleShape& Grid::GetGrid() {
	return rectangle_;
}

std::shared_ptr<City> Grid::GetBelongCity()const {
	return BelongCity.lock();
}

std::shared_ptr<County> Grid::GetBelongCounty()const {
	return BelongCity.lock()->GetCityCounty();
}

std::shared_ptr<Province> Grid::GetBelongProvince()const {
	return BelongCity.lock()->GetCityProvince();
}

std::shared_ptr<Player> Grid::GetBelongPlayer()const {
	return BelongPlayer.lock();
}

std::vector<int>& Grid::Getjsonimagenum() {
	return jsonimagenum;
}

void Grid::SetUp(bool IsCity) {
	if (IsCity) {
		cityup = true;
	}
	else {
		countyup = true;
	}
}

void Grid::SetDown(bool IsCity) {
	if (IsCity) {
		citydown = true;
	}
	else {
		countydown = true;
	}
}

void Grid::SetRight(bool IsCity) {
	if (IsCity) {
		cityright = true;
	}
	else {
		countyright = true;
	}
}

void Grid::SetLeft(bool IsCity) {
	if (IsCity) {
		cityleft = true;
	}
	else {
		countyleft = true;
	}
}

void Grid::JoinCity(const std::shared_ptr<City>& city) {
	BelongCity = city;
	jsoncityid = city->Getid();
}

void Grid::SetNowPosition(float x, float y) {
	NowX = x;
	NowY = y;
	SetNowSpritePosition(x, y);
}

void Grid::SetNowSpritePosition(float x, float y) {
	sprite_.top().setPosition({ x,y });
}

void Grid::SetNowSize(float x, float y) {
	NowSizeX = x;
	NowSizeY = y;
	SetNowSpriteSize(x, y);
}

void Grid::SetNowSpriteSize(float x, float y) {
	float scaleX = x / sprite_.top().getLocalBounds().size.x;
	float scaleY = y / sprite_.top().getLocalBounds().size.y;
	sprite_.top().setScale({ scaleX, scaleY });
}

void Grid::SetSite(const std::string& Site) {
	site = Site;
}

void Grid::SetPlayer(const std::shared_ptr<Player>& player) {
	std::cout << "格子的设置归属函数执行开始-----------------------" << std::endl;
	BelongPlayer = player;
	jsonplayerid = player->GetID();
	sprite_.top().setColor(player->GetColor());
	std::cout << "格子的设置归属函数执行结束" << std::endl;
}

void Grid::SetSprite(const sf::Texture& tex) {
	sprite_.top().setTexture(tex, 1);
	sprite_.top().setOrigin({ sprite_.top().getPosition().x + sprite_.top().getGlobalBounds().size.x / 2,sprite_.top().getPosition().y + sprite_.top().getGlobalBounds().size.y / 2 });
	issprite = 1;
	if (BelongPlayer.lock() != nullptr) {
		sprite_.top().setColor(BelongPlayer.lock()->GetColor());
	}
}

void Grid::SetSprite1(const sf::Texture& tex) {
	sf::Sprite temsprite(texture_);
	sprite_.push(temsprite);
	sprite_.top().setTexture(tex, 1);
	sprite_.top().setOrigin({ sprite_.top().getPosition().x + sprite_.top().getGlobalBounds().size.x / 2,sprite_.top().getPosition().y + sprite_.top().getGlobalBounds().size.y / 2 });
	SetNowSpritePosition(NowX, NowY);
	SetNowSpriteSize(NowSizeX, NowSizeY);
	if (BelongPlayer.lock() != nullptr) {
		sprite_.top().setColor(BelongPlayer.lock()->GetColor());
	}
}

void Grid::SetTemsite(const std::string& tarsite) {
	std::cout << "更换类型函数执行开始-----------------------" << std::endl;
	temsite = site;
	site = tarsite;
	std::cout << "更换类型函数执行结束" << std::endl;
}

void Grid::SetIsMap(){
	IsInMap = 0;
}

void Grid::SetNull(int tary,int tarx){
	isnull = 1;
	x = tarx;
	y = tary;
	rectangle_.setPosition({ static_cast<float>(x),static_cast<float>(y) });
	site = "云朵";
}

void Grid::RemoveSprite() {
	std::cout << "减少顶端图像函数执行开始-----------------------" << std::endl;
	sf::Sprite temsprite = sprite_.top();
	sprite_.pop();
	sprite_.top().setColor(temsprite.getColor());
	sprite_.top().setPosition(temsprite.getPosition());
	SetNowSpriteSize(NowSizeX, NowSizeY);
	if (temsite != "") {
		std::cout << "返回原类型" << std::endl;
		site = temsite;
		temsite = "";
	}
	jsonimagenum.pop_back();
	std::cout << "减少顶端图像函数执行结束" << std::endl;
}

void Grid::SetResource(int levelx) {
	switch (levelx) {
	case 0:
		resourcenum = 25;
		break;
	case 1:
		resourcenum = 150;
		break;
	case 2:
		resourcenum = 450;
		break;
	case 3:
		resourcenum = 800;
		break;
	case 4:
		resourcenum = 1200;
		break;
	case 10:
		if (site == "政府") {
			break;
		}
		if (site == "市井") {
			if (BelongCity.lock()->GetLevel() == 3) {
				resourcenum = 200;
			}
			else if (BelongCity.lock()->GetLevel() == 4) {
				resourcenum = 270;
			}
			else if (BelongCity.lock()->GetLevel() == 5) {
				resourcenum = 350;
			}
			else if (BelongCity.lock()->GetLevel() == 6) {
				resourcenum = 500;
			}
			else if (BelongCity.lock()->GetLevel() == 7) {
				resourcenum = 600;
			}
			else if (BelongCity.lock()->GetLevel() == 8) {
				resourcenum = 900;
			}
			else if (BelongCity.lock()->GetLevel() == 9) {
				resourcenum = 1200;
			}
			break;
		}
		if (site == "城墙") {
			if (BelongCity.lock()->GetLevel() == 3) {
				resourcenum = 100;
			}
			else if (BelongCity.lock()->GetLevel() == 4) {
				resourcenum = 135;
			}
			else if (BelongCity.lock()->GetLevel() == 5) {
				resourcenum = 175;
			}
			else if (BelongCity.lock()->GetLevel() == 6) {
				resourcenum = 250;
			}
			else if (BelongCity.lock()->GetLevel() == 7) {
				resourcenum = 300;
			}
			else if (BelongCity.lock()->GetLevel() == 8) {
				resourcenum = 450;
			}
			else if (BelongCity.lock()->GetLevel() == 9) {
				resourcenum = 600;
			}
			break;
		}
	}
}

void Grid::Setjsonlittlemap() {
	jsonlittlemap = 1 - jsonlittlemap;
}

void Grid::JsonJoinPlayer(std::shared_ptr<Player> player){
	BelongPlayer = player;
	player->GetGridGroup().push_back(shared_from_this());
}

void Grid::JsonJoinCity(std::shared_ptr<City> city){
	BelongCity = city;
	city->GetCityMap().push_back(shared_from_this());
	if (jsonlittlemap) {
		city->GetLittleMap().push_back(shared_from_this());
	}
}

void Grid::fromjson(const nlohmann::json& j) {
	cityup = j["cityup"];
	citydown = j["citydown"];
	cityleft =j["cityleft"];
	cityright = j["cityright"];
	countyup = j["countyup"];
	countydown = j["countydown"];
	countyleft = j["countyleft"];
	countyright = j["countyright"];
	isnull = j["isnull"];
	x = j["x"];
	y = j["y"];
	site = j["site"];
	temsite = j["temsite"];
	resourcenum = j["resourcenum"];
	jsoncityid = j["jsoncityid"];
	jsonplayerid = j["jsonplayerid"];
	jsonimagenum = j["jsonimagenum"].get<std::vector<int>>();
}

nlohmann::json Grid::tojson()const {
	nlohmann::json j;
	j["cityup"] = cityup;
	j["citydown"] = citydown;
	j["cityleft"] = cityleft;
	j["cityright"] = cityright;
	j["countyup"] = countyup;
	j["countydown"] = countydown;
	j["countyleft"] = countyleft;
	j["countyright"] = countyright;
	j["isnull"] = isnull;
	j["x"] = x;
	j["y"] = y;
	j["site"] = site;
	j["temsite"] = temsite;
	j["resourcenum"] = resourcenum;
	j["jsoncityid"] = jsoncityid;
	j["jsonplayerid"] = jsonplayerid;
	j["jsonimagenum"] = jsonimagenum;
	return j;
}

std::string Grid::GetSite()const {
	return site;
}

std::string Grid::GetType()const {
	return GetLastTowChar(site);
}

int Grid::GetResouceNum()const {
	return resourcenum;
}

int Grid::GetX()const {
	return x;
}

int Grid::GetY()const {
	return y;
}

int Grid::Getjsoncityid()const {
	return jsoncityid;
}

int Grid::Getjsonplayerid()const {
	return jsonplayerid;
}

float Grid::GetNowX()const {
	return NowX;
}

float Grid::GetNowY()const {
	return NowY;
}

float Grid::GetNowSizeX()const {
	return NowSizeX;
}

float Grid::GetNowSizeY()const {
	return NowSizeY;
}

bool Grid::IsMap()const{
	return IsInMap;
}

bool Grid::IsSprite()const {
	return issprite;
}

bool Grid::IsNull()const{
	return isnull;
}

bool Grid::isjsonlittlemap()const {
	return jsonlittlemap;
}

bool Grid::IsUp(bool IsCity)const {
	if (IsCity) {
		return cityup;
	}
	else {
		return countyup;
	}
}

bool Grid::IsDown(bool IsCity)const {
	if (IsCity) {
		return citydown;
	}
	else {
		return countydown;
	}
}

bool Grid::IsRight(bool IsCity)const {
	if (IsCity) {
		return cityright;
	}
	else {
		return countyright;
	}
}

bool Grid::IsLeft(bool IsCity)const {
	if (IsCity) {
		return cityleft;
	}
	else {
		return countyleft;
	}
}

sf::Sprite& Grid::GetSprite(){
	return sprite_.top();
}



