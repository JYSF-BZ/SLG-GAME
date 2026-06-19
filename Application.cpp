#include "Application.h"

void Application::INIT() {
	std::cout << "初始化函数执行开始-----------------------" << std::endl;

	//初始化一些图片
	InitPicture(SpriteGroup_[0]);
	InitPicture(SpriteGroup_[2]);
	InitPicture(SpriteGroup_[4]);
	InitOriButton();
	InitStartGameButton();

	//初始化一些库
	InitThingTypeGroup();
	InitBorder();

	//初始化一些按钮
	InitButton(L"Resource/主要模块.txt", ButtonGroup_, ButtonGroup_sprite_);
	InitButton(L"Resource/州.txt", ProvinceButtonGroup_, ProvinceButtonGroup_sprite_);

	std::cout << "初始化函数执行结束" << std::endl;
}

void Application::pixel_to_grid(const sf::Vector2f& temposition, int& x, int& y)const {
	if (pd != 3) {
		std::cout << "不为主要地图界面，退出" << std::endl;
		return;
	}
	x = -1; y = -1;
	for (auto& grid : TarMap) {
		float gridCenterX = grid->GetNowX();
		float gridCenterY = grid->GetNowY();
		float gridHalfWidth = grid->GetNowSizeX() / 2.0f;
		float gridHalfHeight = grid->GetNowSizeY() / 2.0f;
		float left = gridCenterX - gridHalfWidth;
		float right = gridCenterX + gridHalfWidth;
		float top = gridCenterY - gridHalfHeight;
		float bottom = gridCenterY + gridHalfHeight;
		if (temposition.x >= left && temposition.x <= right &&
			temposition.y >= top && temposition.y <= bottom) {
			x = grid->GetX();
			y = grid->GetY();
			return;
		}
	}
}

std::wstring Application::utf8_to_wstring(const std::string& utf8)const{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	std::wstring wstr(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], len);
	return wstr;
}

std::string Application::wstring_to_utf8(const std::wstring& wstr)const{
	if (wstr.empty()) {
		return {};
	}

	// 计算转换后需要的缓冲区大小
	int len = WideCharToMultiByte(
		CP_UTF8,          // 目标编码：UTF-8
		0,                // 无特殊标志
		wstr.c_str(),     // 宽字符串
		static_cast<int>(wstr.size()),  // 长度
		nullptr,          // 输出缓冲区（null表示只计算长度）
		0,                // 缓冲区大小
		nullptr, nullptr
	);

	// 创建目标字符串
	std::string result(len, 0);

	// 执行转换
	WideCharToMultiByte(
		CP_UTF8,
		0,
		wstr.c_str(),
		static_cast<int>(wstr.size()),
		&result[0],
		len,
		nullptr, nullptr
	);

	return result;
}

sf::String Application::utf8_to_String(const std::string& str)const{
	return sf::String::fromUtf8(str.begin(), str.end());
}

std::string Application::String_to_utf8(const sf::String& str)const {
	std::string utf8;
	for (char c : str.toUtf8()) {  // 正确遍历方式
		utf8 += c;
	}
	return utf8;
}

int Application::GetCharCount(const std::wstring& str) const{
	int equivalentLength = 0;
	const wchar_t CHINESE_START = 0x4E00;
	const wchar_t CHINESE_END = 0x9FFF;
	for (wchar_t wc : str) {
		if (wc >= CHINESE_START && wc <= CHINESE_END) {
			equivalentLength += 2;
		}
		else {
			equivalentLength += 1;
		}
	}
	return equivalentLength;
}

bool Application::CheckResourceNum(const Build& tarbuild,const std::shared_ptr<Player>& tarplayer)const {
	std::cout << "检测资源是否足量函数执行开始-----------------------" << std::endl;;
	if (tarbuild.GetLoseResourch()[0] > tarplayer->GetResourceRemain("粮草") || tarbuild.GetLoseResourch()[1] > tarplayer->GetResourceRemain("铁矿") || tarbuild.GetLoseResourch()[2] > tarplayer->GetResourceRemain("木材") || tarbuild.GetLoseResourch()[3] > tarplayer->GetResourceRemain("石料") || tarbuild.GetLoseResourch()[4] > tarplayer->GetResourceRemain("铜币")) {
		std::cout << "检测资源是否足量函数因资源不足执行结束" << std::endl;
		return false;
	}
	std::cout << "检测资源是否足量函数因资源足量执行结束" << std::endl;;
	return true;
}

void Application::SetTextSize(sf::Text& tartext, const sf::Sprite& tarsprite, float padding) {
	// 1. 获取按钮的矩形
	sf::FloatRect btnBounds = tarsprite.getGlobalBounds();
	sf::FloatRect textBounds = tartext.getLocalBounds();

	// 2. 计算可用空间（留一点内边距，不让文字贴边）
	float usableW = btnBounds.size.x - padding * 2;
	float usableH = btnBounds.size.y - padding * 2;

	// 3. 计算缩放比例（取小的那个，保证文字完全放进框里）
	float scaleX = usableW / textBounds.size.x;
	float scaleY = usableH / textBounds.size.y;
	float scale = std::min(scaleX, scaleY);

	// 4. 应用缩放
	tartext.setScale({ scale, scale });

	// 5. 关键：设置文字的原点为它自己的中心，消除自带边距偏移
	tartext.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f,textBounds.position.y + textBounds.size.y / 2.0f });

	// 6. 把文字直接放在按钮的中心位置
	tartext.setPosition({ btnBounds.position.x + btnBounds.size.x / 2.0f,btnBounds.position.y + btnBounds.size.y / 2.0f });
}

bool Application::IsPressButton(const sf::Sprite& tarsprite, const sf::Event::MouseButtonPressed& mouse)const {
	if (mouse.position.x >= tarsprite.getPosition().x && mouse.position.x <= tarsprite.getPosition().x + tarsprite.getGlobalBounds().size.x &&
		mouse.position.y >= tarsprite.getPosition().y && mouse.position.y <= tarsprite.getPosition().y + tarsprite.getGlobalBounds().size.y) {
		return true;
	}
	return false;
}

Application::Application() :
	window_(sf::VideoMode({ sf::VideoMode::getDesktopMode().size.x,sf::VideoMode::getDesktopMode().size.y }), L"静言思飞的练手项目3", sf::Style::Default),
	DataBase_(DataWarehouse::getInstance()),
	WareHouse_(WareHouse::getInstance()),
	Map_(WareHouse_.GetMap()),
	CityGroup_(WareHouse_.GetCityGroup()),
	CountyGroup_(WareHouse_.GetCountyGroup()),
	ProvinceGroup_(WareHouse_.GetProvinceGroup()),
	PlayerGroup_(WareHouse_.GetPlayerGroup()),
	ColorGroup_(WareHouse_.GetColorGroup()),
	IsColorGroup_(WareHouse_.GetIsColorGroup()),
	BuildGroup_(WareHouse_.GetBuildGroup()),
	LandMod(WareHouse_.GetLandMod()),
	SpriteGroup_(WareHouse_.GetSpriteGroup()),
	ChunkGroup_(WareHouse_.GetChunkGroup()),
	CChunkGroup_(WareHouse_.GetCChunkGroup()),
	AllFont_(WareHouse_.GetFontGroup()[0]),
	TextEx_(AllFont_),
	Dialog_(AllFont_)
{
	//一些总体的设置
	window_.setFramerateLimit(60);
	window_.setVerticalSyncEnabled(true);

	//初始化
	INIT();
}

bool Application::IsRunning() const {
	return window_.isOpen();
}

void Application::ProcessEvents() {
	while (const auto event = window_.pollEvent()) {
		event->visit([this](const auto& type) {this->HandleEvent(type); });
	}
}

void Application::Update() {

	if (DialogClock.getElapsedTime().asSeconds() > 5.0f) {
		Dialog_.setString(L"");
	}
	for (int i = 0; i < ResourceGainClockGroup.size(); i++) {
		auto& tarclock = ResourceGainClockGroup[i];
		if (tarclock.getElapsedTime().asSeconds() >= 60.0f) {
			PlayerGroup_[i]->AddRemainResource();
			tarclock.restart();
		}
	}

	if (pd == 3) {
		HelpChangeMap();
		if (pd1 == 308) {
			MainPdButton_[8].setString(TarDialog);
		}
	}
	else if (pd == 5) {
		if (pd1 != 5) {
			UseButton_[UseButton_.size() - 1].setString(TarDialog);
		}
	}
	else if (pd == 6) {
		if (pd1 != 6) {
			UseButton_[UseButton_.size() - 1].setString(TarDialog);
		}
	}
}

void Application::Render() {
	window_.clear();

	if (pd == 0) {
		DRAW(SpriteGroup_[0]);
		DRAW(SpriteGroup_[1]);
	}
	else if (pd == 1) {
		DRAW(SpriteGroup_[2]);
		DRAW(ButtonGroup_sprite_);
		DRAW(ButtonGroup_);
	}
	else if (pd == 2) {
		DRAW(SpriteGroup_[4]);
		if (pd1 == 1) {
			DRAW(ProvinceButtonGroup_sprite_);
			DRAW(ProvinceButtonGroup_);
			DRAW();
		}
		else if (pd1 == 2) {
			DRAW(CountyButtonGroup_sprite_);
			DRAW(CountyButtonGroup_);
			DRAW(TarID);
		}
		else if (pd1 == 3) {
			DRAW(CityButtonGroup_sprite_);
			DRAW(CityButtonGroup_);
			DRAW(TarID);
		}
		else if (pd1 == 4) {
			DRAW(LittleButton_sprite_);
			DRAW(LittleButton_);
			DRAW(TarID);
		}
	}
	else if (pd == 3) {
		DRAW(TarMap);
		DRAW(MainPdButton_sprite_);
		DRAW(MainPdButton_);
		if (pd1 % 10000 / 100 == 10) {
			DRAW(ThingTypeButton_sprite_);
			DRAW(ThingTypeButton_);
			if (pd1 % 100 == 1) {
				DRAW(BuildGroupButton_sprite_);
				DRAW(BuildGroupButton_);
				if (pd5 != -1) {
					DRAW(LoseResourchButton_sprite_);
					DRAW(LoseResourchButton_);
				}
			}
		}
		else if (pd1 % 10000 / 100 == 7) {
			DRAW(PlayerGroupButton_sprite_);
			DRAW(PlayerGroupButton_);
		}
		else if (pd1 % 10000 / 100 == 11) {
			DRAW(ResourceButton_sprite_);
			DRAW(ResourceButton_);
		}
	}
	else if (pd == 4) {
		DRAW(SpriteGroup_[4]);
		DRAW(LittleButton_sprite_);
		DRAW(LittleButton_);
		DRAW(UseButton_sprite_);
		DRAW(UseButton_);
		DRAW(1000000, true);
	}
	else if (pd == 5) {
		DRAW(SpriteGroup_[2]);
		DRAW(LittleButton_sprite_);
		DRAW(LittleButton_);
		DRAW(UseButton_sprite_);
		DRAW(UseButton_);
		if (pd1 == 5) {
			DRAWWindow(UseButton_sprite_[0], DisplayButton_);
		}
	}
	else if (pd == 6) {
		DRAW(SpriteGroup_[2]);
		if (pd1 != 6) {
			DRAW(LittleButton_sprite_);
			DRAW(LittleButton_);
		}
		DRAW(UseButton_sprite_);
		DRAW(UseButton_);
	}
	else if (pd == 7) {
		DRAW(SpriteGroup_[4]);
		DRAW(LittleButton_sprite_);
		DRAW(LittleButton_);
		DRAW(ShowDataGroupButton_sprite_);
		DRAW(ShowDataGroupButton_);
		if (pd1 == 2) {
			DRAW(DataGroupButton_sprite_);
			DRAW(DataGroupButton_);
		}
	}

	if (Dialog_.getString() != L"") {
		window_.draw(Dialog_);
	}

	window_.display();
}

void Application::AddCity(const std::shared_ptr<Player>& player,const std::shared_ptr<City>& city)const {
	std::cout << player->GetName() << "的获取城池函数执行开始-----------------------" << std::endl;
	if (city->GetCityCounty() != nullptr) {
		AddCounty(player, city->GetCityCounty());
	}
	player->GetCityGroup().push_back(city);
	city->SetBelongPlayer(player);
	for (auto& it : city->GetChunkID()) {
		CChunkGroup_[it].SetDirty();
	}
	std::cout << player->GetName() << "的获取城池函数执行结束" << std::endl;
}

void Application::EraseCity(const std::shared_ptr<Player>& player,const std::shared_ptr<City>& city)const {
	std::cout << player->GetName() << "的失去城池函数执行开始-----------------------" << std::endl;
	EraseCounty(player, city->GetCityCounty());
	auto it = find(player->GetCityGroup().begin(), player->GetCityGroup().end(), city);
	if (it != player->GetCityGroup().end()) {
		std::cout << player->GetName() << "的失去城池函数成功在该角色城池库中找到该城池" << std::endl;
		player->GetCityGroup().erase(it);
	}
	if (player->GetCityGroup().size() == 0) {
		std::cout << player->GetName() << "无城池" << std::endl;
		player->SetLive();
	}
	if (player->IsLive() && player->GetBelongCity() == city) {
		std::cout << player->GetName() << "的归属城市丢失，重新设置归属城池" << std::endl;
		player->SetBelongCity(player->GetCityGroup()[0]);
	}
	std::cout << player->GetName() << "的失去城池函数执行结束" << std::endl;
}

void Application::AddCounty(const std::shared_ptr<Player>& player,const std::shared_ptr<County>& county)const {
	std::cout << player->GetName() << "的获取郡县函数执行开始-----------------------" << std::endl;
	AddProvince(player, county->GetCountyProvince());
	for (auto& temcity : county->GetCityGroup()) {
		if (temcity->GetBelongPlayer() != player) {
			std::cout << player->GetName() << "的获取郡县函数因郡县未统一执行结束" << std::endl;
			return;
		}
	}
	county->SetBelongPlayer(player);
	player->GetCountyGroup().push_back(county);
	std::cout << player->GetName() << "的获取郡县函数因郡县统一执行结束" << std::endl;
}

void Application::EraseCounty(const std::shared_ptr<Player>& player,const std::shared_ptr<County>& county)const {
	std::cout << player->GetName() << "的失去郡县函数执行开始-----------------------" << std::endl;
	EraseProvince(player, county->GetCountyProvince());
	for (auto& temcity : county->GetCityGroup()) {
		if (temcity->GetBelongPlayer() != player) {
			std::cout << player->GetName() << "的失去郡县函数因郡县未统一执行结束" << std::endl;
			return;
		}
	}
	auto it = find(player->GetCountyGroup().begin(), player->GetCountyGroup().end(), county);
	if (it != player->GetCountyGroup().end()) {
		player->GetCountyGroup().erase(it);
	}
	std::cout << player->GetName() << "的失去郡县因郡县未统一函数执行结束" << std::endl;
}

void Application::AddProvince(const std::shared_ptr<Player>& player,const std::shared_ptr<Province>& province)const {
	std::cout << player->GetName() << "的获取省份函数执行开始-----------------------" << std::endl;
	for (auto& temcounty : province->GetCountyGroup()) {
		if (temcounty->GetBelongPlayer()!= player) {
			std::cout << player->GetName() << "的获取省份函数因省份未统一执行结束" << std::endl;
			return;
		}
	}
	province->SetBelongPlayer(player);
	player->GetProvinceGroup().push_back(province);
	std::cout << player->GetName() << "的获取省份函数因省份统一执行结束" << std::endl;
}

void Application::EraseProvince(const std::shared_ptr<Player>& player,const std::shared_ptr<Province>& province)const {
	std::cout << player->GetName() << "的失去省份函数执行开始-----------------------" << std::endl;
	for (auto& temcounty : province->GetCountyGroup()) {
		if (temcounty->GetBelongPlayer() != player) {
			std::cout << player->GetName() << "的失去省份函数因省份未统一执行结束" << std::endl;
			return;
		}
	}
	auto it = find(player->GetProvinceGroup().begin(), player->GetProvinceGroup().end(), province);
	if (it != player->GetProvinceGroup().end()) {
		player->GetProvinceGroup().erase(it);
	}
	std::cout << player->GetName() << "的失去省份函数因省份统一执行结束" << std::endl;
}

void Application::UpdateMap() {
	std::cout << "绘制图像一函数执行开始-----------------------" << std::endl;
	ChunkID.clear();
	int HMin = 0, HMax = 0, WMin = 0, WMax = 0, WBase = 0, HBase = 0;
	int PHNum = 0, PWNum = 0;
	int size = Chunk().GetSize();
	int num = (Map_[0].size() + size - 1) / size;
	for (int i = 0;i < WareHouse_.GetChunkGroup().size();i++) {
		ChunkID.push_back(i);
	}
	PHNum = (Map_.size() + size - 1) / size;
	PWNum = (Map_[0].size() + size - 1) / size;
	float YSize = SpriteGroup_[4].getGlobalBounds().size.y * 0.8 / PHNum, XSize = SpriteGroup_[4].getGlobalBounds().size.x * 0.5 / PWNum;
	float Size = std::min(YSize, XSize);
	float Addx = SpriteGroup_[4].getPosition().x - SpriteGroup_[4].getGlobalBounds().size.x * 0.2 + (SpriteGroup_[4].getGlobalBounds().size.x * 0.5 - Size * PWNum) / 2;
	float Addy = SpriteGroup_[4].getPosition().y - SpriteGroup_[4].getGlobalBounds().size.y * 0.4 + (SpriteGroup_[4].getGlobalBounds().size.y * 0.8 - Size * PHNum) / 2;
	for (int i = 0;i < ChunkID.size();i++) {
		int ansx = ChunkID[i] % num - WBase, ansy = ChunkID[i] / num - HBase;
		CChunkGroup_[ChunkID[i]].SetScale(Size / CChunkGroup_[ChunkID[i]].GetSpriteTextureSize());
		CChunkGroup_[ChunkID[i]].SetPosition(Size * static_cast<float>(ansx) + Addx, Size * static_cast<float>(ansy) + Addy);
	}
	std::cout << "绘制图像一函数执行结束" << std::endl;
}

void Application::UpdateMap(int ID) {
	std::cout << "绘制图像二函数执行开始-----------------------" << std::endl;
	ChunkID.clear();
	int HMin = 0, HMax = 0, WMin = 0, WMax = 0, WBase = 0, HBase = 0;
	int PHNum = 0, PWNum = 0;
	int size = Chunk().GetSize();
	int num = (Map_[0].size() + size - 1) / size;
	if (ID == 1000000) {
		std::cout << "绘制全图" << std::endl;
		for (int i = 0;i < WareHouse_.GetChunkGroup().size();i++) {
			ChunkID.push_back(i);
		}
		PHNum = (Map_.size() + size - 1) / size;
		PWNum = (Map_[0].size() + size - 1) / size;
		WBase = 0;
		HBase = 0;
	}
	else {
		Province& temprovince = *GainProvince(ID / 10000 * 10000);
		auto vec = temprovince.GetChunkID();
		ChunkID = std::vector<int>(vec.begin(), vec.end());
		PWNum = temprovince.GetID_Size().Wmax % num - temprovince.GetID_Size().Wmin % num + 1;
		PHNum = temprovince.GetID_Size().Hmax / num - temprovince.GetID_Size().Hmin / num + 1;
		WBase = temprovince.GetID_Size().Wmin % num;
		HBase = temprovince.GetID_Size().Hmin / num;
		if (ID % 100 != 0) {
			std::cout << "绘制县级" << std::endl;
			City& temcity = *GainCity(ID);
			for (auto& it : temcity.GetChunkID()) {
				ChunkGroup_[it].SetDirty();
				ChunkGroup_[it].SetIsOutline(1);
			}
		}
		else if (ID % 10000 != 0) {
			std::cout << "绘制市级" << std::endl;
			County& temcounty = *GainCounty(ID);
			for (auto& it : temcounty.GetChunkID()) {
				ChunkGroup_[it].SetDirty();
				ChunkGroup_[it].SetIsOutline(1);
			}
		}
		else if (ID % 100000 != 0) {
			std::cout << "绘制省级" << std::endl;
			for (auto& it : temprovince.GetChunkID()) {
				ChunkGroup_[it].SetDirty();
				ChunkGroup_[it].SetIsOutline(0);
			}
		}
	}

	float YSize = SpriteGroup_[4].getGlobalBounds().size.y * 0.8 / PHNum, XSize = SpriteGroup_[4].getGlobalBounds().size.x * 0.5 / PWNum;
	float Size = std::round(std::min(YSize, XSize));
	float Addx = SpriteGroup_[4].getPosition().x - SpriteGroup_[4].getGlobalBounds().size.x * 0.2 + (SpriteGroup_[4].getGlobalBounds().size.x * 0.5 - Size * PWNum) / 2;
	float Addy = SpriteGroup_[4].getPosition().y - SpriteGroup_[4].getGlobalBounds().size.y * 0.4 + (SpriteGroup_[4].getGlobalBounds().size.y * 0.8 - Size * PHNum) / 2;
	for (int i = 0;i < ChunkID.size();i++) {
		int ansx = ChunkID[i] % num - WBase, ansy = ChunkID[i] / num - HBase;
		ChunkGroup_[ChunkID[i]].SetScale((Size+0.73f) / ChunkGroup_[ChunkID[i]].GetSpriteTextureSize());
		ChunkGroup_[ChunkID[i]].SetPosition(std::round(Size * ansx + Addx), std::round(Size * ansy + Addy));
	}
	std::cout << "绘制图像二函数执行结束" << std::endl;
}

void Application::UpdateMap(int XStart, int XEnd, int YStart, int YEnd) {
	std::cout << "绘制图像三函数执行开始-----------------------" << std::endl;
	bool judge = 0;
	bool judgex1 = 0, judgex2 = 0, judgey1 = 0, judgey2 = 0;
	for (int i = XStart; i <= XEnd; i++) {
		if (!judgex1 && Map_[YStart][i] != nullptr) {
			judgex1 = 1;
		}
		if (!judgex2 && Map_[YEnd][i] != nullptr) {
			judgex2 = 1;
		}
		if (judgex1 && judgex2) {
			break;
		}
	}
	for (int i = YStart; i <= YEnd; i++) {
		if (!judgey1 && Map_[i][XStart] != nullptr) {
			judgey1 = 1;
		}
		if (!judgey2 && Map_[i][XEnd] != nullptr) {
			judgey2 = 1;
		}
		if (judgey1 && judgey2) {
			break;
		}
	}
	if (judgey1 && judgey2 && judgex1 && judgex2) {
		judge = 1;
	}
	if (judge == 0) {
		std::cout << "绘制图像三函数因图像越界执行结束" << std::endl;
	}
	TarMap.clear();
	float MinX = 0xFFFF, MaxX = 0, MinY = 0xFFFF, MaxY = 0;
	for (int i = YStart; i <= YEnd; i++) {
		for (int j = XStart; j <= XEnd; j++) {
			if (Map_[i][j] == nullptr) {
				continue;
			}
			float temx = Map_[i][j]->GetGrid().getPosition().x;
			float temy = Map_[i][j]->GetGrid().getPosition().y;
			if (temx < MinX) {
				MinX = temx;
			}
			if (temx > MaxX) {
				MaxX = temx;
			}
			if (temy < MinY) {
				MinY = temy;
			}
			if (temy > MaxY) {
				MaxY = temy;
			}
		}
	}
	TarMap.reserve((XEnd - XStart + 1) * (YEnd - YStart + 1));
	int W = MaxX - MinX;
	int H = MaxY - MinY;
	float windowUsableW = SpriteGroup_[2].getGlobalBounds().size.x * 1.2;
	float windowUsableH = SpriteGroup_[2].getGlobalBounds().size.y * 1.2;

	float scaleW = windowUsableW / W;
	float scaleH = windowUsableH / H;
	float globalScale = std::min(scaleW, scaleH);
	float cityMidX = MinX + W / 2.0f;
	float cityMidY = MinY + H / 2.0f;

	int WindowMidX = SpriteGroup_[2].getPosition().x;
	int WindowMidY = SpriteGroup_[2].getPosition().y;
	for (int i = YStart; i <= YEnd; i++) {
		for (int j = XStart; j <= XEnd; j++) {
			if (Map_[i][j] == nullptr) {
				continue;
			}
			auto& tar = Map_[i][j]->GetGrid();
			sf::RectangleShape& grid = Map_[i][j]->GetGrid();
			grid.setScale({ globalScale,globalScale });
			float gridRelX = grid.getPosition().x - cityMidX;
			float gridRelY = grid.getPosition().y - cityMidY;
			float scaledRelX = gridRelX * globalScale;
			float scaledRelY = gridRelY * globalScale;
			float finalX = WindowMidX + scaledRelX;
			float finalY = WindowMidY + scaledRelY;
			Map_[i][j]->SetNowPosition(finalX, finalY);
			Map_[i][j]->SetNowSize(grid.getSize().x * globalScale, grid.getSize().y * globalScale);
			TarMap.emplace_back(Map_[i][j]);
		}
	}
	std::cout << "绘制图像三函数执行结束" << std::endl;
}

void Application::Change(const std::shared_ptr<Grid>& onlytarmap) {
	std::cout << "点击更换目标格子函数执行开始-----------------------" << std::endl;
	if (onlytarmap->IsNull()) {
		std::cout << "点击更换目标格子函数因无可选格子执行结束" << std::endl;
		return;
	}
	if (temtarmap_->GetBelongPlayer() != nullptr) {
		temtarmap_->GetGrid().setOutlineThickness(-0.1);
		temtarmap_->GetGrid().setOutlineColor(temtarmap_->GetBelongPlayer()->GetColor());
		temtarmap_->GetSprite().setColor(temtarmap_->GetBelongPlayer()->GetColor());
	}
	else {
		temtarmap_->GetGrid().setOutlineThickness(0);
		temtarmap_->GetSprite().setColor(sf::Color::White);
	}
	onlytarmap->GetGrid().setOutlineColor(sf::Color::White);
	onlytarmap->GetGrid().setOutlineThickness(-0.1);
	onlytarmap->GetSprite().setColor({ 175, 122, 66 });
	temtarmap_ = onlytarmap;
	for (int i = 1; i <= 6; i++) {
		std::string tarstr;
		switch (i) {
		case 1:
			tarstr = onlytarmap->GetBelongProvince()->GetName() + "-" + onlytarmap->GetBelongCounty()->GetName() + "-" + onlytarmap->GetBelongCity()->GetName();
			break;
		case 2:
			tarstr = "(" + std::to_string(onlytarmap->GetX()) + "," + std::to_string(onlytarmap->GetY()) + ")";
			break;
		case 3:
			tarstr = std::to_string(onlytarmap->GetBelongCity()->GetLevel());
			break;
		case 4:
			tarstr = onlytarmap->GetBelongCity()->GetSite();
			break;
		case 5:
			tarstr = onlytarmap->GetSite();
			break;
		case 6:
			if (onlytarmap->GetBelongPlayer() != nullptr) {
				tarstr = onlytarmap->GetBelongPlayer()->GetName();
			}
			else {
				tarstr = "中立";
			}
			break;
		}
		MainPdButton_[i].setString(utf8_to_String(tarstr));
	}
	std::cout << "点击更换格子函数执行结束" << std::endl;
}

std::shared_ptr<Grid> Application::GainGrid(int tarx, int tary)const {
	int nowy = tary - HMIN;
	int nowx = tarx - WMIN;
	if (nowy < 0 || nowy >= Map_.size()) {
		return nullptr;
	}
	if (nowx < 0 || nowx >= Map_[nowy].size()) {
		return nullptr;
	}
	return Map_[nowy][nowx];
}

std::shared_ptr<City> Application::GainCity(int tarID)const {
	for (int i = 0; i < CityGroup_.size(); i++) {
		if (CityGroup_[i]->GetID() == tarID) {
			return CityGroup_[i];
		}
	}
	return nullptr;
}

std::shared_ptr<City> Application::GainCity(const std::string& tarname)const {
	for (int i = 0; i < CityGroup_.size(); i++) {
		if (CityGroup_[i]->GetName() == tarname) {
			return CityGroup_[i];
		}
	}
	return nullptr;
}

std::shared_ptr<County> Application::GainCounty(int tarID)const {
	for (int i = 0; i < CountyGroup_.size(); i++) {
		if (CountyGroup_[i]->GetID() == tarID) {
			return CountyGroup_[i];
		}
	}
	return nullptr;
}

std::shared_ptr<County> Application::GainCounty(const std::string& tarname)const {
	for (int i = 0; i < CountyGroup_.size(); i++) {
		if (CountyGroup_[i]->GetName() == tarname) {
			return CountyGroup_[i];
		}
	}
	return nullptr;
}

std::shared_ptr<Province> Application::GainProvince(int tarID)const {
	for (int i = 0; i < ProvinceGroup_.size(); i++) {
		if (ProvinceGroup_[i]->GetID() == tarID) {
			return ProvinceGroup_[i];
		}
	}
	return nullptr;
}

std::shared_ptr<Province> Application::GainProvince(const std::string& tarname)const {
	for (int i = 0; i < ProvinceGroup_.size(); i++) {
		if (ProvinceGroup_[i]->GetName() == tarname) {
			return ProvinceGroup_[i];
		}
	}
	return nullptr;
}

std::shared_ptr<Player> Application::GainPlayer(const std::string& tarname) const{
	for (int i = 0; i < PlayerGroup_.size(); i++) {
		if (PlayerGroup_[i]->GetName() == tarname) {
			return PlayerGroup_[i];
		}
	}
	return nullptr;
}

void Application::InitThingTypeGroup() {
	std::cout << "导入事件类型集函数执行开始-----------------------" << std::endl;
	std::ifstream file(L"Resource/事件类型.txt");
	file.imbue(std::locale::classic());
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		std::istringstream lineStream(line);
		std::string Type;
		lineStream >> Type;
		ThingTypeGroup_.push_back(Type);
	}
	file.close();
	std::cout << "导入事件类型集函数执行结束" << std::endl;
}

void Application::InitBorder() {
	std::cout << "导入地图边界函数执行开始-----------------------" << std::endl;
	HMIN = WareHouse_.GetHMIN();
	WMIN = WareHouse_.GetWMIN();
	HMAX = WareHouse_.GetHMAX();
	WMAX = WareHouse_.GetWMAX();
	std::cout << "导入地图边界函数执行结束" << std::endl;
}

void Application::InitShowDataGroupTableGroup() {
	std::cout << "表格类型一览函数执行开始-----------------------" << std::endl;
	ShowDataGroupButton_.clear();
	ShowDataGroupButton_sprite_.clear();
	std::vector<std::string>temtextx = { "县城","郡","州", "<---","--->" };
	const float startx = LittleButton_sprite_[0].getPosition().x, starty = LittleButton_sprite_[0].getPosition().y;
	const float leny = LittleButton_sprite_[0].getGlobalBounds().size.y;
	for (int i = 0; i < temtextx.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setString(utf8_to_String(temtextx[i]));
		temsprite.setPosition({ static_cast<float>(startx),static_cast<float>(starty + leny * 1.1 * (i + 1)) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		ShowDataGroupButton_sprite_.push_back(temsprite);
		ShowDataGroupButton_.push_back(temtext);
	}
	std::cout << "表格类型一览函数执行结束" << std::endl;
}

void Application::InitDataGroup() {
	std::cout << "绘制表格函数执行开始-----------------------" << std::endl;
	DataGroupButton_.clear();
	DataGroupButton_sprite_.clear();
	if (ChartName == "县城") {
		DataBase_.GetColumn() = {
		{"1","-","归属州","归属郡","县"},
		{"0","地位","等级","县令","坐标"}
		};
		DataBase_.GetAs() = { "归属" };
	}
	else if (ChartName == "郡") {
		DataBase_.GetColumn() = {
		{"1","-","归属州","郡"},
		{"0","太守"}
		};
		DataBase_.GetAs() = { "归属" };
	}
	else if (ChartName == "州") {
		DataBase_.GetColumn() = {
		{"0","州"},{"0","刺史" }
		};
		DataBase_.GetAs() = { "归属" };
	}
	DataBase_.GetLimit() = { start,len };
	DataBase_.ShowRowData(ChartName);

	auto field_names = DataBase_.GetFieldNames();
	auto rows_data = DataBase_.GetResultRows();
	int col_num = field_names.size();
	if (col_num == 0) {
		return;
	}

	int startx = LittleButton_sprite_[0].getPosition().x, starty = LittleButton_sprite_[0].getPosition().y;
	int lenx = LittleButton_sprite_[0].getGlobalBounds().size.x, leny = LittleButton_sprite_[0].getGlobalBounds().size.y;
	for (int i = 0; i < col_num; i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setString(utf8_to_String(field_names[i]));
		temsprite.setPosition({ static_cast<float>(startx + lenx * 1.1 * (i + 1)),static_cast<float>(starty) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		SetTextSize(temtext, temsprite, 12.0f);
		DataGroupButton_sprite_.push_back(temsprite);
		DataGroupButton_.push_back(temtext);
	}
	sf::Sprite temsprite = SpriteGroup_[3];
	int ansy = 1;
	for (auto& row : rows_data) {
		for (int i = 0; i < col_num; i++) {
			sf::Text temtext(AllFont_);
			if (row[i] == "") {
				temtext.setString(utf8_to_String("无"));
			}
			else {
				temtext.setString(utf8_to_String(row[i]));
			}
			SetTextSize(temtext, temsprite, 12.0f);
			temtext.setPosition({ static_cast<float>(DataGroupButton_[i].getPosition().x),static_cast<float>(DataGroupButton_[i].getPosition().y + DataGroupButton_[i].getGlobalBounds().size.y * 2.0 * ansy) });
			temtext.setFillColor(sf::Color::Black);
			DataGroupButton_.push_back(temtext);
		}
		ansy++;
	}
	std::cout << "绘制表格函数执行结束" << std::endl;
}

void Application::InitText(sf::Text& tartext)const {
	std::cout << "初始化文字函数执行开始-----------------------" << std::endl;
	tartext.setFillColor(sf::Color::Red);
	tartext.setCharacterSize(50.0f);
	sf::FloatRect tem = tartext.getLocalBounds();
	tartext.setOrigin({ tem.position.x + tem.size.x / 2,tem.position.y + tem.size.y / 2 });
	tartext.setPosition({ static_cast<float>(window_.getSize().x / 2),static_cast<float>(window_.getSize().y / 2) });
	std::cout << "初始化文字函数执行结束" << std::endl;
}

void Application::InitPicture(sf::Sprite& Tarsprite)const {
	std::cout << "初始化图像函数执行开始-----------------------" << std::endl;
	float windowx = window_.getSize().x;
	float windowy = window_.getSize().y;
	float tarx = Tarsprite.getGlobalBounds().size.x;
	float tary = Tarsprite.getGlobalBounds().size.y;
	float finscale = std::min(windowx / tarx, windowy / tary);
	Tarsprite.setScale({ finscale,finscale });
	Tarsprite.setOrigin({ Tarsprite.getLocalBounds().size.x / 2,Tarsprite.getLocalBounds().size.y / 2 });
	Tarsprite.setPosition({ static_cast<float>(window_.getSize().x / 2),static_cast<float>(window_.getSize().y / 2) });
	std::cout << "初始化图像函数执行结束" << std::endl;
}

void Application::InitOriButton() {
	std::cout << "初始化模板按钮函数执行开始-----------------------" << std::endl;
	sf::FloatRect sprite3 = SpriteGroup_[3].getGlobalBounds();

	float Changex = 150.f / SpriteGroup_[3].getGlobalBounds().size.x;
	float Changey = 50.f / SpriteGroup_[3].getGlobalBounds().size.y;
	SpriteGroup_[3].setScale({ Changex,Changey });
	Changex = SpriteGroup_[3].getGlobalBounds().size.x;
	Changey = SpriteGroup_[3].getGlobalBounds().size.y;
	std::cout << "初始化模板函数执行结束" << std::endl;
}

void Application::InitStartGameButton() {
	std::cout << "初始化开始按钮函数执行开始-----------------------" << std::endl;
	SpriteGroup_[1].setPosition({ SpriteGroup_[0].getPosition().x + SpriteGroup_[0].getLocalBounds().size.x * 3 / 10,SpriteGroup_[0].getPosition().y + SpriteGroup_[0].getLocalBounds().size.y * 2 / 10 });
	float temscale = SpriteGroup_[0].getGlobalBounds().size.x / 1000;
	SpriteGroup_[1].setScale({ temscale, temscale });
	std::cout << "初始化开始按钮函数执行结束" << std::endl;
}

void Application::InitButton() {
	std::cout << "设置返回按钮执行开始-----------------------" << std::endl;
	LittleButton_.clear();
	LittleButton_sprite_.clear();
	int ansx = 0, ansy = 0;
	sf::Sprite temsprite = SpriteGroup_[3];
	sf::Text temtext(AllFont_);
	temtext.setFillColor(sf::Color::Black);
	float temlengthx = temsprite.getGlobalBounds().size.x * 1.1, temlengthy = temsprite.getGlobalBounds().size.y * 1.1;
	temsprite.setPosition({ static_cast<float>(SpriteGroup_[2].getPosition().x - SpriteGroup_[2].getGlobalBounds().size.x / 2 + (temlengthx)*ansx),static_cast<float>(SpriteGroup_[2].getPosition().y - SpriteGroup_[2].getGlobalBounds().size.y / 2 + temlengthy * ansy) });
	temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
	temtext.setString(L"返回");
	LittleButton_sprite_.push_back(temsprite);
	LittleButton_.push_back(temtext);
	std::cout << "设置返回按钮执行结束" << std::endl;
}

void Application::InitButton(const std::shared_ptr<Grid>& onlytarmap) {
	std::cout << "设置主要地图初始按钮执行开始-----------------------" << std::endl;
	std::wstring temname = L"";
	std::wstring temname1 = L"";
	if (MainPdButton_.size() != 0) {
		temname = MainPdButton_[7].getString();
		temname1 = MainPdButton_[10].getString();
	}
	int temx = onlytarmap->GetX();
	int temy = onlytarmap->GetY();
	int MinH = onlytarmap->GetBelongProvince()->GetOnlyMapSize().Hmin;
	int MinW = onlytarmap->GetBelongProvince()->GetOnlyMapSize().Wmin;
	int winx = temx - MinW, winy = temy - MinH;
	temtarmap_ = onlytarmap;
	onlytarmap->GetGrid().setOutlineColor(sf::Color::White);
	onlytarmap->GetGrid().setOutlineThickness(-0.1);
	onlytarmap->GetSprite().setColor({ 175, 122, 66 });
	MainPdButton_.clear();
	MainPdButton_sprite_.clear();
	int ansx = -1, ansy = 0;
	for (int i = 0; i < 12; i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setFillColor(sf::Color::Black);
		float temlengthx = temsprite.getGlobalBounds().size.x * 1.1, temlengthy = temsprite.getGlobalBounds().size.y * 1.1;
		if (temlengthx * (ansx + 2) + temsprite.getGlobalBounds().size.x > window_.getSize().x) {
			ansx = 0;
			ansy++;
		}
		else {
			ansx++;
		}
		temsprite.setPosition({ static_cast<float>(window_.getPosition().x + temlengthx * ansx),static_cast<float>(window_.getPosition().y + temlengthy * ansy) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		std::string tarstr = "aaa";
		switch (i) {
		case 0:
			temtext.setString(L"返回");
			break;
		case 1:
			tarstr = onlytarmap->GetBelongProvince()->GetName() + "-" + onlytarmap->GetBelongCounty()->GetName() + "-" + onlytarmap->GetBelongCity()->GetName();
			break;
		case 2:
			tarstr = "(" + std::to_string(onlytarmap->GetY()) + "," + std::to_string(onlytarmap->GetX()) + ")";
			break;
		case 3:
			tarstr = std::to_string(onlytarmap->GetBelongCity()->GetLevel());
			break;
		case 4:
			tarstr = onlytarmap->GetBelongCity()->GetSite();
			break;
		case 5:
			tarstr = onlytarmap->GetSite();
			break;
		case 6:
			if (Map_[winy][winx]->GetBelongPlayer() == nullptr) {
				temtext.setString(L"中立");
			}
			else {
				tarstr = Map_[winy][winx]->GetBelongPlayer()->GetName();
			}
			break;
		case 7:
			if (temname != L"") {
				temtext.setString(temname);
			}
			else {
				temtext.setString(L"当前角色");
			}
			break;
		case 8:
			temtext.setString(L"定位:");
			break;
		case 9:
			temtext.setString(L"重置");
			break;
		case 10:
			if (MainPdButton_[7].getString() == L"当前角色") {
				temtext.setString(L"事件类型");
			}
			else {
				temtext.setString(temname1);
			}
			break;
		case 11:
			temtext.setString(L"资源列表");
			break;
		}
		if (temtext.getString() == L"") {
			temtext.setString(utf8_to_String(tarstr));
		}
		if (i == 1) {
			SetTextSize(temtext, temsprite);
		}
		MainPdButton_sprite_.push_back(temsprite);
		MainPdButton_.push_back(temtext);
	}
	std::cout << "设置主要地图初始按钮执行结束" << std::endl;
}

void Application::InitButton(const std::wstring& FilePath, std::vector<sf::Text>& tarbuttongroup, std::vector<sf::Sprite>& tarbuttonggroup_sprite)const {
	std::cout << "导入文字函数执行开始-----------------------" << std::endl;
	tarbuttongroup.clear();
	tarbuttonggroup_sprite.clear();
	int ansx = 0, ansy = 0;
	std::ifstream file(FilePath);
	file.imbue(std::locale::classic());
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == L'#') {
			continue;
		}
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setFillColor(sf::Color::Black);
		float temlengthx = temsprite.getGlobalBounds().size.x * 1.1, temlengthy = temsprite.getGlobalBounds().size.y * 1.1;
		int num = SpriteGroup_[0].getGlobalBounds().size.y / (temsprite.getGlobalBounds().size.y * 1.1);
		temsprite.setPosition({ static_cast<float>(SpriteGroup_[0].getPosition().x - SpriteGroup_[0].getGlobalBounds().size.x / 2 + temlengthx * ansx),static_cast<float>(SpriteGroup_[0].getPosition().y - SpriteGroup_[0].getGlobalBounds().size.y / 2 + temlengthy * ansy) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		if (ansy < num) {
			ansy++;
		}
		else {
			ansy = 0;
			ansx++;
		}
		temtext.setString(sf::String::fromUtf8(line.begin(), line.end()));
		tarbuttonggroup_sprite.push_back(temsprite);
		tarbuttongroup.push_back(temtext);
	}
	file.close();
	std::cout << "导入文字函数执行结束" << std::endl;
}

void Application::InitButton(std::vector<sf::Text>& tarbuttongroup, std::vector<sf::Sprite>& tarbuttonggroup_sprite) {
	std::cout << "导入郡县文字函数执行开始-----------------------" << std::endl;
	tarbuttongroup.clear();
	tarbuttonggroup_sprite.clear();
	int ansx = 0, ansy = 0;
	std::vector<std::string> targroup;
	if (TarID % 10000 == 0 && TarID / 10000 != 0) {
		for (auto& temcounty : GainProvince(TarID)->GetCountyGroup()) {
			targroup.push_back(temcounty->GetName());
		}
	}
	else if (TarID % 100 == 0 && TarID / 100 != 0) {
		for (auto& temcity : GainCounty(TarID)->GetCityGroup()) {
			targroup.push_back(temcity->GetName());
		}
	}
	for (int i = 0;i <= targroup.size();i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setFillColor(sf::Color::Black);
		if (i == 0) {
			temtext.setString(L"返回");
		}
		else {
			temtext.setString(utf8_to_wstring(targroup[i - 1]));
		}
		float temlengthx = temsprite.getGlobalBounds().size.x * 1.1, temlengthy = temsprite.getGlobalBounds().size.y * 1.1;
		int num = SpriteGroup_[0].getGlobalBounds().size.y / (temsprite.getGlobalBounds().size.y * 1.1);
		temsprite.setPosition({ static_cast<float>(SpriteGroup_[0].getPosition().x - SpriteGroup_[0].getGlobalBounds().size.x / 2 + temlengthx * ansx),static_cast<float>(SpriteGroup_[0].getPosition().y - SpriteGroup_[0].getGlobalBounds().size.y / 2 + temlengthy * ansy) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		if (ansy < num) {
			ansy++;
		}
		else {
			ansy = 0;
			ansx++;
		}
		tarbuttonggroup_sprite.push_back(temsprite);
		tarbuttongroup.push_back(temtext);
	}
	std::cout << "导入郡县文字函数执行结束" << std::endl;
}

void Application::InitButton(int ID) {
	std::cout << "设置县级界面函数执行开始-----------------------" << std::endl;
	LittleButton_sprite_.clear();
	LittleButton_.clear();
	int ansx = 0, ansy = 0, ans = 2;
	while (ans > 0) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setFillColor(sf::Color::Black);
		float temlengthx = temsprite.getGlobalBounds().size.x * 1.1, temlengthy = temsprite.getGlobalBounds().size.y * 1.1;
		temsprite.setPosition({ static_cast<float>(SpriteGroup_[0].getPosition().x - SpriteGroup_[0].getGlobalBounds().size.x / 2 + temlengthx * ansx),static_cast<float>(SpriteGroup_[0].getPosition().y - SpriteGroup_[0].getGlobalBounds().size.y / 2 + temlengthy * ansy) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		if (ansy < 20) {
			ansy++;
		}
		else {
			ansy = 0;
			ansx++;
		}
		if (ans == 2) {
			temtext.setString(L"返回");
		}
		else {
			City Co = *GainCity(ID);
			std::string temstr = Co.GetName();
			temtext.setString(sf::String::fromUtf8(temstr.begin(), temstr.end()));
		}
		LittleButton_sprite_.push_back(temsprite);
		LittleButton_.push_back(temtext);
		ans--;
	}
	std::cout << "设置县级界面函数执行结束" << std::endl;
}

void Application::InitDisplayButton() {
	std::cout << "设置对话框函数执行开始-----------------------" << std::endl;
	UseButton_sprite_.clear();
	UseButton_.clear();
	int ansy = 0;
	int BaseX = LittleButton_sprite_[0].getPosition().x;
	int BaseY = LittleButton_sprite_[0].getPosition().y + LittleButton_sprite_[0].getGlobalBounds().size.y;
	for (int i = 0; i < PlayerGroup_.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext = TextEx_;
		temsprite.setScale({ 0.5f,0.5f });
		float tarx = BaseX;
		float tary = BaseY + ansy * temsprite.getGlobalBounds().size.y + 2;
		temsprite.setPosition({ tarx,tary });
		temsprite.setColor(PlayerGroup_[i]->GetColor());
		temtext.setString(utf8_to_String(PlayerGroup_[i]->GetName()));
		temtext.setFillColor(sf::Color::Black);
		temtext.setPosition({ tarx,tary });
		UseButton_sprite_.push_back(temsprite);
		UseButton_.push_back(temtext);
		ansy++;
	}
	std::cout << "设置对话框函数执行结束" << std::endl;
}

void Application::InitButtonFindPlayer(const std::shared_ptr<Player>& player) {
	std::cout << "展示玩家详细信息函数执行开始-----------------------" << std::endl;
	UseButton_sprite_.clear();
	UseButton_.clear();
	for (int i = 0; i < 8; i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext = TextEx_;
		if (i == 0) {
			std::cout << "主框" << std::endl;
			float temscalex = SpriteGroup_[2].getGlobalBounds().size.x / (2 * temsprite.getLocalBounds().size.x);
			float temscaley = SpriteGroup_[2].getGlobalBounds().size.y / (2 * temsprite.getLocalBounds().size.y);
			temsprite.setScale({ temscalex,temscaley });
			float tarx = SpriteGroup_[2].getPosition().x - temsprite.getGlobalBounds().size.x / 2;
			float tary = SpriteGroup_[2].getPosition().y - temsprite.getGlobalBounds().size.y / 2;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"占有城池一览：");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx + 20,tary + 24 });
		}
		else if (i == 1) {
			std::cout << "玩家名称" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[0].getPosition().x - temsprite.getGlobalBounds().size.x - 10;
			float tary = UseButton_sprite_[0].getPosition().y;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(utf8_to_String(player->GetName()));
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 2) {
			std::cout << "玩家存活状态" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[1].getPosition().x;
			float tary = UseButton_sprite_[1].getPosition().y + UseButton_sprite_[1].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			if (player->IsLive()) {
				temtext.setString(L"存活");
			}
			else {
				temtext.setString(L"死亡");
			}
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 3) {
			std::cout << "玩家归属" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[2].getPosition().x;
			float tary = UseButton_sprite_[2].getPosition().y + UseButton_sprite_[2].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			if (player->GetBelongCity() == nullptr) {
				temtext.setString(L"归属:无");
			}
			else {
				std::wstring str = utf8_to_String(player->GetBelongCity()->GetName());
				temtext.setString(L"归属:" + str);
			}
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 4) {
			std::cout << "玩家首都" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[3].getPosition().x;
			float tary = UseButton_sprite_[3].getPosition().y + UseButton_sprite_[3].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			if (player->GetCapital() == nullptr) {
				temtext.setString(L"首都:无");
			}
			else {
				std::wstring str = utf8_to_String(player->GetCapital()->GetName());
				temtext.setString(L"首都:" + str);
			}
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 5) {
			std::cout << "县级" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[0].getPosition().x + UseButton_sprite_[0].getGlobalBounds().size.x + 10;
			float tary = UseButton_sprite_[0].getPosition().y;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"县");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 6) {
			std::cout << "市级" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[5].getPosition().x;
			float tary = UseButton_sprite_[5].getPosition().y + UseButton_sprite_[5].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"郡");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 7) {
			std::cout << "省级" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[6].getPosition().x;
			float tary = UseButton_sprite_[6].getPosition().y + UseButton_sprite_[6].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"州");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		UseButton_sprite_.push_back(temsprite);
		UseButton_.push_back(temtext);
	}
	std::cout << "展示玩家详细信息函数执行结束" << std::endl;
}

void Application::InitChooseColorButton(const std::shared_ptr<Player>& player) {
	std::cout << "设置选择颜色函数执行开始-----------------------" << std::endl;
	UseButton_sprite_.clear();
	UseButton_.clear();
	int ansx = 0, ansy = 0;
	float BaseX = 0, BaseY = 0;
	for (int i = 0; i <= ColorGroup_.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext = TextEx_;
		if (i == 0) {
			std::cout << "主框" << std::endl;
			float temscalex = SpriteGroup_[2].getGlobalBounds().size.x / (2 * temsprite.getLocalBounds().size.x);
			float temscaley = SpriteGroup_[2].getGlobalBounds().size.y / (2 * temsprite.getLocalBounds().size.y);
			temsprite.setScale({ temscalex,temscaley });
			float tarx = SpriteGroup_[2].getPosition().x - temsprite.getGlobalBounds().size.x / 2;
			float tary = SpriteGroup_[2].getPosition().y - temsprite.getGlobalBounds().size.y / 2;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"请选择你的颜色：");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx + 20,tary + 24 });
			UseButton_sprite_.push_back(temsprite);
			UseButton_.push_back(temtext);
			BaseX = UseButton_sprite_[0].getPosition().x;
			BaseY = UseButton_sprite_[0].getPosition().y + UseButton_sprite_[0].getGlobalBounds().size.y;
		}
		else {
			std::cout << "小框" << i << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = BaseX + ansx * temsprite.getGlobalBounds().size.x;
			float tary = BaseY + ansy * temsprite.getGlobalBounds().size.y;
			if (ansx < 3) {
				ansx++;
			}
			else {
				ansx = 0;
				ansy++;
			}
			temsprite.setPosition({ tarx,tary });
			if (IsColorGroup_[i - 1]) {
				temsprite.setColor(sf::Color::Transparent);
			}
			else {
				temsprite.setColor(ColorGroup_[i - 1]);
			}
			UseButton_sprite_.push_back(temsprite);
		}
	}
	std::cout << "设置选择颜色函数执行结束" << std::endl;
}

void Application::InitDialog(const std::string& Str) {
	std::cout << "设置对话框按钮函数执行开始-----------------------" << std::endl;
	DisplayButton_.clear();
	UseButton_sprite_.clear();
	UseButton_.clear();
	for (int i = 0; i < 5; i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext = TextEx_;
		if (i == 0) {
			std::cout << "主框" << std::endl;
			float temscalex = SpriteGroup_[2].getGlobalBounds().size.x / (2 * temsprite.getLocalBounds().size.x);
			float temscaley = SpriteGroup_[2].getGlobalBounds().size.y / (2 * temsprite.getLocalBounds().size.y);
			temsprite.setScale({ temscalex,temscaley });
			float tarx = SpriteGroup_[2].getPosition().x - temsprite.getGlobalBounds().size.x / 2;
			float tary = SpriteGroup_[2].getPosition().y - temsprite.getGlobalBounds().size.y / 2;
			temsprite.setPosition({ tarx,tary });
			std::wstring str = utf8_to_String(Str);
			temtext.setString(sf::String::fromUtf8(Str.begin(), Str.end()));
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx + 20,tary + 24 });
		}
		else if (i == 1) {
			std::cout << "确定按钮" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[0].getPosition().x + UseButton_sprite_[0].getGlobalBounds().size.x + 10;
			float tary = UseButton_sprite_[0].getPosition().y;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"确定");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 2) {
			std::cout << "清空按钮" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[1].getPosition().x;
			float tary = UseButton_sprite_[1].getPosition().y + UseButton_sprite_[1].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"清空");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 3) {
			std::cout << "撤销按钮" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x / 4, UseButton_sprite_[0].getScale().y / 4 });
			float tarx = UseButton_sprite_[2].getPosition().x;
			float tary = UseButton_sprite_[2].getPosition().y + UseButton_sprite_[2].getGlobalBounds().size.y + 2;
			temsprite.setPosition({ tarx,tary });
			temtext.setString(L"撤销");
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx,tary });
		}
		else if (i == 4) {
			std::cout << "对话框" << std::endl;
			temsprite.setScale({ UseButton_sprite_[0].getScale().x, UseButton_sprite_[0].getScale().y });
			float tarx = SpriteGroup_[2].getPosition().x - temsprite.getGlobalBounds().size.x / 2;
			float tary = SpriteGroup_[2].getPosition().y;
			temtext.setString(TarDialog);
			temtext.setFillColor(sf::Color::Black);
			temtext.setPosition({ tarx + 10,tary });
		}
		if (i < 4) {
			UseButton_sprite_.push_back(temsprite);
		}
		UseButton_.push_back(temtext);
	}
	std::cout << "设置对话框按钮函数执行结束" << std::endl;
}

void Application::InitNum() {
	std::cout << "设置列表单页函数执行开始-----------------------" << std::endl;
	float leny = DataGroupButton_sprite_[0].getGlobalBounds().size.y;
	len = (SpriteGroup_[4].getPosition().y + SpriteGroup_[4].getGlobalBounds().size.y / 2) / leny - 1;
	PreLen = len;
	std::cout << "设置列表单页函数执行结束" << std::endl;
}

void Application::InitChooseThingTypeButton(int startx,int endx,int starty,int endy) {
	std::cout << "初始化事件类型函数执行开始-----------------------" << std::endl;
	ThingTypeButton_sprite_.clear();
	ThingTypeButton_.clear();
	for (int i = 0; i < ThingTypeGroup_.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setString(utf8_to_String(ThingTypeGroup_[i]));
		temsprite.setPosition({ static_cast<float>(startx),static_cast<float>(starty + (endy - starty) * (i + 1)) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		ThingTypeButton_sprite_.push_back(temsprite);
		ThingTypeButton_.push_back(temtext);
	}
	std::cout << "初始化事件类型函数执行结束" << std::endl;
}

void Application::InitBuildGroupButton(int startx, int endx, int starty, int endy) {
	std::cout << "初始化建筑物集函数执行开始-----------------------" << std::endl;
	BuildGroupButton_sprite_.clear();
	BuildGroupButton_.clear();
	for (int i = 0; i < BuildGroup_.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setString(utf8_to_String(BuildGroup_[i].GetName()));
		temsprite.setPosition({ static_cast<float>(endx),static_cast<float>(starty + (endy - starty) * i) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		BuildGroupButton_sprite_.push_back(temsprite);
		BuildGroupButton_.push_back(temtext);
	}
	std::cout << "初始化建筑物集函数执行结束" << std::endl;
}

void Application::InitPlayerGroupButton(int startx, int endx, int starty, int endy) {
	std::cout << "初始化玩家集函数执行开始-----------------------" << std::endl;
	PlayerGroupButton_sprite_.clear();
	PlayerGroupButton_.clear();
	for (int i = 0; i < PlayerGroup_.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setString(utf8_to_String(PlayerGroup_[i]->GetName()));
		temsprite.setPosition({ static_cast<float>(startx),static_cast<float>(starty + (endy - starty) * (i + 1)) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		PlayerGroupButton_sprite_.push_back(temsprite);
		PlayerGroupButton_.push_back(temtext);
	}
	std::cout << "初始化玩家集函数执行结束" << std::endl;
}

void Application::InitResourceButton(int startx, int endx, int starty, int endy) {
	std::cout << "初始化资源列表函数执行开始-----------------------" << std::endl;
	ResourceButton_sprite_.clear();
	ResourceButton_.clear();
	std::vector<std::wstring>temarray = { L"粮草",L"铁矿",L"木材",L"石料",L"铜币" };
	if (MainPdButton_[7].getString() == L"当前角色") {
		std::cout << "初始化资源列表函数因无当前角色执行结束" << std::endl;
		return;
	}
	std::shared_ptr<Player> tarplayer = GainPlayer(String_to_utf8(MainPdButton_[7].getString()));
	for (int i = 0; i < temarray.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		sf::Text temtext1(AllFont_);
		sf::Text temtext2(AllFont_);
		temtext.setString(temarray[i]);
		temtext1.setString(std::to_string(tarplayer->GetResourceRemain(String_to_utf8(temarray[i]))));
		temtext2.setString("+" + std::to_string(tarplayer->GetResourceGain(String_to_utf8(temarray[i]))));
		temsprite.setPosition({ static_cast<float>(startx),static_cast<float>(starty + (endy - starty) * (i + 1)) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext1.setPosition({ temtext.getPosition().x + temtext.getGlobalBounds().size.x + 20,temtext.getPosition().y });
		temtext2.setPosition({ temtext1.getPosition().x,temtext1.getPosition().y + temtext1.getGlobalBounds().size.y + 5 });
		temtext.setCharacterSize(temsprite.getGlobalBounds().size.y - 20);
		temtext1.setCharacterSize(temtext.getCharacterSize() / 2);
		temtext2.setCharacterSize(temtext.getCharacterSize() / 2);
		temtext.setFillColor(sf::Color::Black);
		temtext1.setFillColor(sf::Color::Black);
		temtext2.setFillColor(sf::Color::Black);
		ResourceButton_sprite_.push_back(temsprite);
		ResourceButton_.push_back(temtext);
		ResourceButton_.push_back(temtext1);
		ResourceButton_.push_back(temtext2);
	}
	std::cout << "初始化资源列表函数执行结束" << std::endl;
}

void Application::InitLoseDisplayButton(int startx, int endx,int starty, int endy, const Build& tembuild) {
	std::cout << "初始化建筑物消耗函数执行开始-----------------------" << std::endl;
	LoseResourchButton_sprite_.clear();
	LoseResourchButton_.clear();
	std::vector<std::wstring>temarray = { L"粮草",L"铁矿",L"木材",L"石料",L"铜币" };
	std::vector<int>temlose = { tembuild.GetLoseResourch()[0],tembuild.GetLoseResourch()[1],tembuild.GetLoseResourch()[2],tembuild.GetLoseResourch()[3],tembuild.GetLoseResourch()[4] };
	for (int i = 0; i < temarray.size(); i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		sf::Text temtext1(AllFont_);
		temtext.setString(temarray[i]);
		temtext1.setString(std::to_string(temlose[i]));
		temsprite.setPosition({ static_cast<float>(endx),static_cast<float>(starty + (endy - starty) * i) });
		temtext.setCharacterSize(temsprite.getGlobalBounds().size.y - 20);
		temtext1.setCharacterSize(temtext1.getCharacterSize());
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext1.setPosition({ temtext.getPosition().x + temtext.getGlobalBounds().size.x + 20,temtext.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		temtext1.setFillColor(sf::Color::Black);
		LoseResourchButton_sprite_.push_back(temsprite);
		LoseResourchButton_.push_back(temtext);
		LoseResourchButton_.push_back(temtext1);
	}
	std::cout << "初始化建筑物消耗函数执行结束" << std::endl;
}

void Application::DRAW(const std::vector<std::shared_ptr<Grid>>& tarmap) {
	if (pd == 3) {
		for (int i = 0; i < tarmap.size(); i++) {
			window_.draw(tarmap[i]->GetSprite());
		}
	}
}

void Application::DRAW(std::vector<Grid>& tarmap) {
	for (int i = 0; i < tarmap.size(); i++) {
		window_.draw(tarmap[i].GetGrid());
	}
}

void Application::DRAW(const sf::Sprite& tarsprite) {
	window_.draw(tarsprite);
}

void Application::DRAW(int ID, bool Ctype) {
	if (Ctype) {
		for (auto& tarc : ChunkID) {
			CChunkGroup_[tarc].Draw(window_, ID, Ctype);
		}
	}
	else {
		for (auto& tarc : ChunkID) {
			ChunkGroup_[tarc].Draw(window_, ID, Ctype);
		}
	}
	
}

void Application::HelpChangeMap() {
	if (IsDraging && MouseClock.getElapsedTime().asSeconds() > 0.08f) {
		sf::Vector2f EndPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window_));
		int temx1, temx2, temy1, temy2;
		pixel_to_grid(StartPosition, temx1, temy1);
		pixel_to_grid(EndPosition, temx2, temy2);
		if (temx1 == -1 || temx2 == -1 || temy1 == -1 || temy2 == -1) {
			std::cout << "找不到格子" << std::endl;
			return;
		}
		PreX -= temx2 - temx1;
		PreY -= temy2 - temy1;
		if (PreX < 0 || PreX + PY > WMAX - WMIN || PreY < HMIN || PreY + PY / 2 > HMAX) {
			PreX += temx2 - temx1;
			PreY += temy2 - temy1;
			std::cout << "越界" << std::endl;
			return;
		}
		UpdateMap(PreX, PreX + PY, PreY, PreY + PY / 2);
		StartPosition = EndPosition;
	}
}

void Application::HelpFindCity() {
	std::cout << "定位城池函数执行开始-----------------------" << std::endl;
	std::string temname = "";
	for (auto& templayer : PlayerGroup_) {
		if (!templayer->IsLive()) {
			continue;
		}
		if (templayer->GetName() == wstring_to_utf8(TarDialog)) {
			temname = templayer->GetBelongCity()->GetName();
			break;
		}
	}
	std::shared_ptr<City> temcity = nullptr;
	if (temname == "") {
		std::cout << "未找到玩家" << std::endl;
		temcity = GainCity(wstring_to_utf8(TarDialog));
	}
	else {
		std::cout << "找到玩家" << std::endl;
		temcity = GainCity(temname);
	}
	if (temcity != nullptr) {
		std::cout << "找到城池" << std::endl;
		int temx = temcity->GetMainX() - WMIN;
		int temy = temcity->GetMainY() - HMIN;
		PreX = temx - PYB / 2;
		PreY = temy - PYB / 4;;
		PY = PYB;
		if (PreX < 0) {
			std::cout << "稍作偏移" << std::endl;
			PreX += 10;
		}
		UpdateMap(PreX, PreX + PY, PreY, PreY + PY / 2);
		if (TarMap.size() != 0) {
			Change(TarMap[(PY + 1) * (temy - PreY) + temx - PreX]);
		}
	}
	MainPdButton_sprite_[8].setColor(sf::Color::White);
	pd1 = 1;
	TarDialog = L"";
	MainPdButton_[8].setString(L"定位:");
	std::cout << "定位城池函数执行结束" << std::endl;
}

void Application::HelpFind() {
	std::cout << "查找角色函数执行开始-----------------------" << std::endl;
	if (TarDialog != L"") {
		bool xxx = 0;
		for (int i = 0; i < PlayerGroup_.size(); i++) {
			if (utf8_to_String(PlayerGroup_[i]->GetName()) == TarDialog) {
				pd1 = 5;
				TarPlayer = PlayerGroup_[i];
				InitButtonFindPlayer(TarPlayer);
				xxx = 1;
				break;
			}
		}
		if (xxx == 0) {
			TarDialog = L"无该角色，再次查找请清空";
		}
	}
	std::cout << "查找角色函数执行结束" << std::endl;
}

void Application::HelpCreate() {
	std::cout << "创建角色函数执行开始-----------------------" << std::endl;
	if (ColorGroup_.size() == 0) {
		std::cout << "创建角色函数因颜色缺失而执行结束" << std::endl;
		TarDialog = L"颜色不足，无法创建";
		return;
	}
	if (TarDialog != L"") {
		bool xxx = 0;
		for (int i = 0; i < PlayerGroup_.size(); i++) {
			if (utf8_to_String(PlayerGroup_[i]->GetName()) == TarDialog) {
				std::cout << "角色名重合" << std::endl;
				TarDialog = L"已有该角色，请清空后重新创建";
				xxx = 1;
				break;
			}
		}
		for (int i = 0; i < CityGroup_.size(); i++) {
			if (CityGroup_[i]->GetName() == wstring_to_utf8(TarDialog)) {
				std::cout << "与城市名重合" << std::endl;
				TarDialog = L"与城市名重合，请清空后重新创建";
				xxx = 1;
				break;
			}
		}
		if (xxx == 0) {
			if (GetCharCount(TarDialog) > 8) {
				std::cout << "长度过长" << std::endl;
				TarDialog = L"长度过长，再次创建请清空";
			}
			else {
				auto player = std::make_shared<Player>(wstring_to_utf8(TarDialog), PlayerGroup_.size());
				PlayerGroup_.push_back(player);
				ResourceGainClockGroup.emplace_back();
				ResourceGainClockGroup[ResourceGainClockGroup.size() - 1].restart();
				TaxClockGroup.emplace_back();
				TaxClockGroup[TaxClockGroup.size() - 1].restart();
				TarDialog = L"成功创建，再次创建请清空";
				pd1 = 6;
				InitChooseColorButton(PlayerGroup_[PlayerGroup_.size() - 1]);
			}
		}
	}
	std::cout << "创建角色函数执行结束" << std::endl;
}

void Application::HelpBackSpace() {
	if (!TarDialog.empty()) {
		TarDialog.pop_back();
	}
}

void Application::HelpSetColor(int x) {
	std::cout << "设置颜色函数执行开始-----------------------" << std::endl;
	std::shared_ptr<Player> player = PlayerGroup_[PlayerGroup_.size() - 1];
	std::shared_ptr<City> tarcity = nullptr;
	while (true) {
		int h = rand() % CityGroup_.size();
		tarcity = CityGroup_[h];
		if (!tarcity->IsBelong() && (tarcity->GetSite() == "大型县城" || tarcity->GetSite() == "中型县城" || tarcity->GetSite() == "小型县城")) {
			break;
		}
	}
	player->SetBelongCity(tarcity);
	player->SetColor(ColorGroup_[x - 1], x - 1);
	for (auto& targrid : tarcity->GetLittleMap()) {
		HelpJoinGrid(player, targrid);
	}
	IsColorGroup_[x - 1] = true;
	std::cout << "设置颜色函数执行结束" << std::endl;
}

void Application::HelpCountDataGroupNum() {
	std::cout << "计数MySQL函数执行开始-----------------------" << std::endl;
	ChartMax = 0;
	DataGroupButton_.clear();
	DataGroupButton_sprite_.clear();
	if (ChartName == "县城") {
		DataBase_.GetColumn() = {
		{"1","-","归属州","归属郡","县"},
		{"0","地位","等级","县令","坐标"}
		};
		DataBase_.GetAs() = { "归属" };
	}
	else if (ChartName == "郡") {
		DataBase_.GetColumn() = {
		{"1","-","归属州","郡"},
		{"0","太守"}
		};
		DataBase_.GetAs() = { "归属" };
	}
	else if (ChartName == "州") {
		DataBase_.GetColumn() = {
		{"0","州"},{"0","刺史"}
		};
		DataBase_.GetAs() = { "归属" };
	}
	DataBase_.ShowRowData(ChartName);

	// 使用封装好的统一接口获取数据
	auto field_names = DataBase_.GetFieldNames();
	auto rows_data = DataBase_.GetResultRows();
	int col_num = field_names.size();
	if (col_num == 0){
		return;
	}

	int startx = LittleButton_sprite_[0].getPosition().x, starty = LittleButton_sprite_[0].getPosition().y;
	int lenx = LittleButton_sprite_[0].getGlobalBounds().size.x, leny = LittleButton_sprite_[0].getGlobalBounds().size.y;
	for (int i = 0; i < col_num; i++) {
		sf::Sprite temsprite = SpriteGroup_[3];
		sf::Text temtext(AllFont_);
		temtext.setString(utf8_to_String(field_names[i]));
		temsprite.setPosition({ static_cast<float>(startx + lenx * 1.1 * (i + 1)),static_cast<float>(starty) });
		temtext.setPosition({ temsprite.getPosition().x,temsprite.getPosition().y });
		temtext.setFillColor(sf::Color::Black);
		SetTextSize(temtext, temsprite, 12.0f);
		DataGroupButton_sprite_.push_back(temsprite);
		DataGroupButton_.push_back(temtext);
	}
	sf::Sprite temsprite = SpriteGroup_[3];
	MYSQL_ROW row;
	int ansy = 1;
	for (auto& row : rows_data) {
		for (int i = 0; i < col_num; i++) {
			sf::Text temtext(AllFont_);
			if (row[i] == "") {
				temtext.setString(utf8_to_String("无"));
			}
			else {
				temtext.setString(utf8_to_String(row[i]));
			}
			SetTextSize(temtext, temsprite, 12.0f);
			temtext.setPosition({ static_cast<float>(DataGroupButton_sprite_[i].getPosition().x),static_cast<float>(DataGroupButton_sprite_[i].getPosition().y + DataGroupButton_sprite_[i].getGlobalBounds().size.y * 1.1 * ansy) });
			temtext.setFillColor(sf::Color::Black);
			DataGroupButton_.push_back(temtext);
		}
		ChartMax++;
		ansy++;
	}
	InitNum();
	std::cout << "计数MySQL函数执行结束" << std::endl;
}

void Application::HelpJoinGrid(const std::shared_ptr<Player>& tarplayer,const std::shared_ptr<Grid>& targrid) {
	std::cout << "加入格子函数执行开始-----------------------" << std::endl;
	std::shared_ptr<Player> tarplayer1 = nullptr;
	if (targrid->GetBelongPlayer() != nullptr) {
		std::cout << "防御方存在" << std::endl;
		tarplayer1 = PlayerGroup_[targrid->GetBelongPlayer()->GetID()];
		if (tarplayer1->GetName() == tarplayer->GetName()) {
			std::cout << "进攻方与防御方相同，退出" << std::endl;
			return;
		}
		auto it = std::find(tarplayer1->GetGridGroup().begin(), tarplayer1->GetGridGroup().end(), targrid);
		if (it != tarplayer1->GetGridGroup().end()) {
			tarplayer1->EraseGainResource(targrid);
			tarplayer1->GetGridGroup().erase(it);
		}
	}
	std::cout << "防御方不存在（不考虑政府情况）" << std::endl;
	if (targrid->GetSite() == "政府") {
		std::cout << "进攻地区为政府" << std::endl;
		if (tarplayer1 != nullptr) {
			EraseCity(tarplayer1, targrid->GetBelongCity());
		}
		AddCity(tarplayer, targrid->GetBelongCity());
	}
	targrid->SetPlayer(tarplayer);
	tarplayer->GetGridGroup().emplace_back(targrid);
	tarplayer->AddGainResource(targrid);
	Map_[targrid->GetY() - HMIN][targrid->GetX() - WMIN]->GetGrid().setOutlineColor(tarplayer->GetColor());
	Map_[targrid->GetY() - HMIN][targrid->GetX() - WMIN]->GetGrid().setOutlineThickness(-0.1);
	Map_[targrid->GetY() - HMIN][targrid->GetX() - WMIN]->SetPlayer(tarplayer);
	std::cout << "加入格子函数执行结束" << std::endl;
}

void Application::HelpFightWin() {
	std::cout << "获胜函数执行开始-----------------------" << std::endl;
	std::shared_ptr<Player> tarplayer = GainPlayer(String_to_utf8(MainPdButton_[7].getString()));
	HelpJoinGrid(tarplayer, temtarmap_);
	std::cout << "获胜函数执行结束" << std::endl;
}

void Application::HelpFightLose() {
	std::cout << "失败函数执行开始-----------------------" << std::endl;

	std::cout << "失败函数执行结束" << std::endl;
}

void Application::HelpFightHelp() {
	int random = rand() % 1;
	if (random == 0) {
		std::cout << "获胜" << std::endl;
		HelpFightWin();
	}
	else {
		std::cout << "失败" << std::endl;
		HelpFightLose();
	}
}

void Application::HelpFight() {
	std::cout << "辅助进攻函数执行开始-----------------------" << std::endl;
	if (temtarmap_->GetSite() == "深山" || temtarmap_->GetSite() == "山脉" || temtarmap_->GetSite() == "云朵" || temtarmap_->GetSite() == "要塞") {
		std::cout << "辅助进攻函数因特殊地势而执行结束" << std::endl;
		return;
	}
	else {
		int temx = temtarmap_->GetX(), temy = temtarmap_->GetY();
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				std::shared_ptr<Grid> targrid = GainGrid(temx + i, temy + j);
				if (targrid->IsNull()) {
					continue;
				}
				else {
					if (targrid->GetBelongPlayer() == nullptr) {
						continue;
					}
					if (targrid->GetBelongPlayer()->GetName() == String_to_utf8(MainPdButton_[7].getString())) {
						if (targrid->GetSite() == "河流") {
							continue;
						}
						HelpFightHelp();
						std::cout << "辅助进攻函数执行结束" << std::endl;
						return;
					}
				}

			}
		}
	}
	std::cout << "辅助进攻函数因周围无归属地块而执行结束" << std::endl;
}

void Application::HelpBuildHotel() {
	std::cout << "建造要塞函数执行开始-----------------------" << std::endl;
	if (temtarmap_->GetSite() == "深山" || temtarmap_->GetSite() == "山脉" || temtarmap_->GetSite() == "河流" || temtarmap_->GetSite() == "要塞" ||
		temtarmap_->GetSite() == "政府" || temtarmap_->GetSite() == "市井" || temtarmap_->GetSite() == "城墙" || temtarmap_->GetSite() == "桥梁" ||
		temtarmap_->GetSite() == "云朵") {
		std::cout << "建造要塞函数因建设地区不为土地无法建造而执行结束" << std::endl;
		return;
	}
	if (temtarmap_->GetBelongPlayer() == nullptr) {
		std::cout << "建造要塞函数因该地块无主无法建造而执行结束" << std::endl;
		return;
	}
	std::shared_ptr<Player> tarplayer = GainPlayer(String_to_utf8(MainPdButton_[7].getString()));
	if (temtarmap_->GetBelongPlayer()->GetName() != tarplayer->GetName()) {
		std::cout << "建造要塞函数因该地块归属与当前角色不符无法建造而执行结束" << std::endl;
		return;
	}
	if (!CheckResourceNum(BuildGroup_[0], tarplayer)) {
		std::cout << "建造要塞函数因资源不足无法建造而执行结束" << std::endl;
		Dialog_.setString(L"资源不足！请稍后再试");
		DialogClock.restart();
		InitText(Dialog_);
		return;
	}
	temtarmap_->GetBelongPlayer()->EraseRemainResource("要塞");
	int temx = temtarmap_->GetX() - WMIN;
	int temy = temtarmap_->GetY() - HMIN;
	Map_[temy][temx]->SetSprite1(LandMod[0][2]);
	Map_[temy][temx]->Getjsonimagenum().push_back(2);
	Map_[temy][temx]->SetTemsite("要塞");
	std::cout << "建造要塞函数执行结束" << std::endl;
}

void Application::HelpBuildBridge() {
	std::cout << "建造桥梁函数执行开始-----------------------" << std::endl;
	if (temtarmap_->GetSite() != "河流") {
		std::cout << "建造要塞函数因建设地区不为河流无法建造而执行结束" << std::endl;
		return;
	}
	if (temtarmap_->GetBelongPlayer() == nullptr) {
		std::cout << "建造河流函数因该地块无主无法建造而执行结束" << std::endl;
		return;
	}
	std::shared_ptr<Player> tarplayer = GainPlayer(String_to_utf8(MainPdButton_[7].getString()));
	if (temtarmap_->GetBelongPlayer()->GetName() != tarplayer->GetName()) {
		std::cout << "建造河流函数因该地块归属与当前角色不符无法建造而执行结束" << std::endl;
		return;
	}
	if (!CheckResourceNum(BuildGroup_[1], tarplayer)) {
		std::cout << "建造河流函数因资源不足无法建造而执行结束" << std::endl;
		Dialog_.setString(L"资源不足！请稍后再试");
		DialogClock.restart();
		InitText(Dialog_);
		return;
	}
	temtarmap_->GetBelongPlayer()->EraseRemainResource("桥梁");
	int temx = temtarmap_->GetX() - WMIN;
	int temy = temtarmap_->GetY() - HMIN;
	Map_[temy][temx]->SetSprite1(LandMod[0][3]);
	Map_[temy][temx]->Getjsonimagenum().push_back(3);
	Map_[temy][temx]->SetTemsite("桥梁");
	std::cout << "建造桥梁函数执行结束" << std::endl;
}

void Application::HelpMove() {

}

void Application::HelpDemolish() {
	std::cout << "拆除函数执行开始-----------------------" << std::endl;
	if (temtarmap_->GetSite() != "要塞" && temtarmap_->GetSite() != "桥梁") {
		std::cout << "拆除函数因无建筑需要拆除而执行结束" << std::endl;
		return;
	}
	if (temtarmap_->GetBelongPlayer() == nullptr) {
		std::cout << "拆除函数因该地块无归属而执行结束" << std::endl;
		return;
	}
	if (temtarmap_->GetBelongPlayer()->GetName() != String_to_utf8(MainPdButton_[7].getString())) {
		std::cout << "拆除函数因拆除地块归属和当前角色不一致而执行结束" << std::endl;
		return;
	}
	int temx = temtarmap_->GetX() - WMIN;
	int temy = temtarmap_->GetY() - HMIN;
	Map_[temy][temx]->RemoveSprite();
	std::cout << "拆除函数执行结束" << std::endl;
}

void Application::HelpTax() {
	std::cout << "收税函数执行开始-----------------------" << std::endl;
	std::shared_ptr<Player> tarplayer = GainPlayer(String_to_utf8(MainPdButton_[7].getString()));
	if (TaxClockGroup[tarplayer->GetID()].getElapsedTime().asSeconds() < 10.0f) {
		Dialog_.setString(L"时间未到，剩余" + std::to_wstring(1200.0f - TaxClockGroup[tarplayer->GetID()].getElapsedTime().asSeconds()) + L"秒");
		DialogClock.restart();
		InitText(Dialog_);
		std::cout << "收税函数因时间未到执行结束" << std::endl;
		return;
	}
	TaxClockGroup[tarplayer->GetID()].restart();
	tarplayer->AddRemainMoneyResource();
	std::cout << "收税函数执行结束" << std::endl;
}

void Application::Help_Pd3_Pd13xx10(const sf::Event::MouseButtonPressed& mouse) {
	std::cout << "辅助事件类型启动函数执行开始-----------------------" << std::endl;
	for (int i = 0; i < ThingTypeButton_sprite_.size(); i++) {
		if (IsPressButton(ThingTypeButton_sprite_[i], mouse)) {
			switch (i) {
			case 0:
				HelpFight();
				break;
			case 2:
				HelpMove();
				break;
			case 3:
				HelpDemolish();
				break;
			case 4:
				HelpTax();
				break;
			}
		}
	}
	std::cout << "辅助事件类型启动函数执行结束" << std::endl;
}

void Application::HelpPd0(const sf::Event::MouseButtonPressed& mouse) {
	if (IsPressButton(SpriteGroup_[1], mouse)) {
		pd = 1;
	}
}

void Application::HelpPd1(const sf::Event::MouseButtonPressed& mouse) {
	for (int i = 0; i < ButtonGroup_sprite_.size(); i++) {
		if (IsPressButton(ButtonGroup_sprite_[i], mouse)) {
			pd = i + 2;
			if (pd == 2) {
				TarID = 1000000;
				UpdateMap(TarID);
				pd1 = 1;
			}
			else if (pd == 3) {
				UpdateMap(PreX, PreX + PY, PreY, PreY + PY / 2);
				if (PY == PYB) {
					if (temtarmap_ == nullptr) {
						if (TarMap.size() != 0) {
							int temx = CityGroup_[0]->GetMainX() - WMIN, temy = CityGroup_[0]->GetMainY() - HMIN;
							InitButton(TarMap[(PY + 1) * (temy - PreY) + temx - PreX]);
						}
					}
					else {
						InitButton(temtarmap_);
					}
				}
			}
			else if (pd == 4) {
				InitButton();
				UpdateMap();
				InitDisplayButton();
			}
			else if (pd == 5) {
				TarDialog = L"";
				InitButton();
				InitDialog("请输入您要查询的玩家名称");
			}
			else if (pd == 6) {
				TarDialog = L"";
				InitButton();
				InitDialog("请输入您的名称：");
			}
			else if (pd == 7) {
				pd1 = 2;
				InitButton();
				InitShowDataGroupTableGroup();
				start = PreStart, len = PreLen;
				ChartName = "县城";
				HelpCountDataGroupNum();
				InitDataGroup();
			}
		}
	}
}

void Application::HelpPd2(const sf::Event::MouseButtonPressed& mouse) {
	//包含各州界面
	if (pd1 == 1) {
		for (int i = 0; i < ProvinceButtonGroup_sprite_.size(); i++) {
			if (IsPressButton(ProvinceButtonGroup_sprite_[i], mouse)) {
				if (i == 0) {
					pd = 1;
					pd1 = 1;
				}
				else {
					pd1 = 2;
					pd2 = i;
					TarID = 1000000 + pd2 * 10000;
					if (TarID != 1020000) {
						pd = 2;
						pd1 = 1;
					}
					else {
						InitButton(CountyButtonGroup_, CountyButtonGroup_sprite_);
						std::shared_ptr<Province> temprovince = GainProvince(TarID);
						for (auto& it : temprovince->GetChunkID()) {
							ChunkGroup_[it].SetDirty();
						}
						UpdateMap(TarID);
					}
				}
			}
		}
	}
	//包含各郡界面
	else if (pd1 == 2) {
		for (int i = 0; i < CountyButtonGroup_sprite_.size(); i++) {
			if (IsPressButton(CountyButtonGroup_sprite_[i], mouse)) {
				if (i == 0) {
					pd1 = 1;
					std::shared_ptr<Province> temprovince = GainProvince(TarID);
					for (auto& it : temprovince->GetChunkID()) {
						ChunkGroup_[it].SetDirty();
					}
					TarID = 1000000;
					UpdateMap(TarID);
				}
				else {
					CityButtonGroup_sprite_.clear();
					CityButtonGroup_.clear();
					pd1 = 3;
					pd3 = i;
					TarID = 1000000 + pd2 * 10000 + pd3 * 100;
					InitButton(CityButtonGroup_, CityButtonGroup_sprite_);
					UpdateMap(TarID);
				}
			}
		}
	}
	//包含各县界面
	else if (pd1 == 3) {
		for (int i = 0; i < CityButtonGroup_sprite_.size(); i++) {
			if (IsPressButton(CityButtonGroup_sprite_[i], mouse)) {
				if (i == 0) {
					pd1 = 2;
					TarID = 1000000 + pd2 * 10000;
					InitButton(CountyButtonGroup_, CountyButtonGroup_sprite_);
					UpdateMap(TarID);
				}
				else {
					pd1 = 4;
					pd4 = i;
					TarID = 1000000 + pd2 * 10000 + pd3 * 100 + pd4;
					InitButton(TarID);
					UpdateMap(TarID);
				}
			}
		}
	}
	//县城
	else if (pd1 == 4) {
		if (IsPressButton(LittleButton_sprite_[0], mouse)) {
			pd1 = 3;
			TarID = 1000000 + pd2 * 10000 + pd3 * 100;
			InitButton(CityButtonGroup_, CityButtonGroup_sprite_);
			UpdateMap(TarID);
		}
	}
}

void Application::HelpPd3(const sf::Event::MouseButtonPressed& mouse) {
	//先检测上方按键
	for (int i = 0; i < MainPdButton_.size(); i++) {
		sf::Sprite& temsprite = MainPdButton_sprite_[i];
		sf::Text& temtext = MainPdButton_[i];
		if (IsPressButton(temsprite, mouse)) {
			switch (i) {
			case 0:
				pd1 = 1;
				pd = 1;
				return;
			case 8:
				if (temsprite.getColor() == sf::Color::White) {
					TarDialog = L"";
					temsprite.setColor(sf::Color::Red);
					temtext.setString(L"");
					pd1 = 308;
				}
				else if (temsprite.getColor() == sf::Color::Red) {
					HelpFindCity();
				}
				return;
			case 9:
				PY = PYB;
				UpdateMap(PreX, PreX + PY, PreY, PreY + PY / 2);
				if (TarMap.size() != 0) {
					Change(TarMap[(PY / 4 - 1) * (PY + 1) + PY / 2]);
				}
				return;
			}
			break;
		}
	}
	//检测角色点击
	if (pd1 % 10000 / 100 == 7) {
		for (int i = 0; i < PlayerGroupButton_.size(); i++) {
			if (IsPressButton(PlayerGroupButton_sprite_[i], mouse)) {
				MainPdButton_[7].setString(PlayerGroupButton_[i].getString());
				return;
			}
		}
	}
	//再检测事件类型
	if (pd1 % 10000 / 100 == 10) {
		const int temh = pd1 % 100;
		if (temh == 0 && mouse.position.x >= MainPdButton_sprite_[10].getPosition().x && mouse.position.x <= MainPdButton_sprite_[10].getPosition().x + MainPdButton_sprite_[10].getGlobalBounds().size.x) {
			for (int j = 0; j < ThingTypeButton_sprite_.size(); j++) {
				if (mouse.position.y >= MainPdButton_sprite_[10].getPosition().y && mouse.position.y <= MainPdButton_sprite_[10].getPosition().y + MainPdButton_sprite_[10].getGlobalBounds().size.y * (ThingTypeButton_sprite_.size() + 1)) {
					Help_Pd3_Pd13xx10(mouse);
					return;
				}
			}
		}
		else if (temh == 1) {
			for (int j = 0; j < BuildGroupButton_sprite_.size(); j++) {
				if (IsPressButton(BuildGroupButton_sprite_[j], mouse)) {
					if (j == 0) {
						HelpBuildHotel();
					}
					else if (j == 1) {
						HelpBuildBridge();
					}
					return;
				}
			}
		}
	}
	//最后检测地图
	IsDraging = 1;
	MouseClock.restart();
	StartPosition = { static_cast<float>(mouse.position.x),static_cast<float>(mouse.position.y) };
	for (int i = 0; i < TarMap.size(); i++) {
		if (mouse.position.x >= TarMap[i]->GetNowX() - TarMap[i]->GetNowSizeX() / 2 && mouse.position.x <= TarMap[i]->GetNowX() + TarMap[i]->GetNowSizeX() / 2 &&
			mouse.position.y >= TarMap[i]->GetNowY() - TarMap[i]->GetNowSizeY() / 2 && mouse.position.y <= TarMap[i]->GetNowY() + TarMap[i]->GetNowSizeY() / 2) {
			if (PY == PYB && !TarMap[i]->IsNull()) {
				Change(TarMap[i]);
			}
			break;
		}
	}
}

void Application::HelpPd4(const sf::Event::MouseButtonPressed& mouse) {
	if (IsPressButton(LittleButton_sprite_[0], mouse)) {
		pd1 = 1;
		pd = 1;
	}
}

void Application::HelpPd5(const sf::Event::MouseButtonPressed& mouse) {
	if (IsPressButton(LittleButton_sprite_[0], mouse)) {
		pd1 = 1;
		pd = 1;
	}
	if (pd1 != 5) {
		for (int i = 0; i < UseButton_sprite_.size(); i++) {
			if (IsPressButton(UseButton_sprite_[i], mouse)) {
				if (i == 1) {
					if (TarDialog != L"") {
						HelpFind();
					}
				}
				else if (i == 2) {
					TarDialog = L"";
				}
				else if (i == 3) {
					HelpBackSpace();
				}
			}
		}
	}
	else {
		for (int i = 5; i <= 7; i++) {
			if (IsPressButton(UseButton_sprite_[i], mouse)) {
				if (i == 5) {
					InitDisplayButton(TarPlayer, TarPlayer->GetCityGroup());
				}
				else if (i == 6) {
					InitDisplayButton(TarPlayer, TarPlayer->GetCountyGroup());
				}
				else if (i == 7) {
					InitDisplayButton(TarPlayer, TarPlayer->GetProvinceGroup());
				}
			}
		}
	}
}

void Application::HelpPd6(const sf::Event::MouseButtonPressed& mouse) {
	if (pd1 != 6) {
		if (IsPressButton(LittleButton_sprite_[0], mouse)) {
			pd1 = 1;
			pd = 1;
		}
		else {
			for (int i = 0; i < UseButton_sprite_.size(); i++) {
				if (IsPressButton(UseButton_sprite_[i], mouse)) {
					if (i == 1) {
						HelpCreate();
					}
					else if (i == 2) {
						TarDialog = L"";
					}
					else if (i == 3) {
						HelpBackSpace();
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < UseButton_sprite_.size(); i++) {
			if (IsPressButton(UseButton_sprite_[i], mouse)) {
				if (i != 0) {
					if (IsColorGroup_[i - 1]) {
						continue;
					}
					HelpSetColor(i);
					pd1 = 1;
					pd = 1;
					break;
				}
			}
		}
	}
}

void Application::HelpPd7(const sf::Event::MouseButtonPressed& mouse) {
	if (pd1 == 2) {
		if (IsPressButton(LittleButton_sprite_[0], mouse)) {
			pd1 = 1;
			pd = 1;
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (IsPressButton(ShowDataGroupButton_sprite_[i], mouse)) {
					if (i < 3) {
						ChartName = String_to_utf8(ShowDataGroupButton_[i].getString());
						start = PreStart, len = PreLen;
						HelpCountDataGroupNum();
						InitDataGroup();
						IsAsc = 0;
					}
					else if (i == 3) {
						if (IsAsc) {
							DataBase_.GetOrderByColumn().push_back(OrderByColumn);
							DataBase_.GetOrderByIsAsc().push_back(OrderByIsAsc);
						}
						start -= len;
						if (start < 0) {
							start += len;
						}
						else {
							InitDataGroup();
						}
					}
					else if (i == 4) {
						if (IsAsc) {
							DataBase_.GetOrderByColumn().push_back(OrderByColumn);
							DataBase_.GetOrderByIsAsc().push_back(OrderByIsAsc);
						}
						start += len;
						if (start >= ChartMax) {
							start -= len;
						}
						else {
							InitDataGroup();
						}
					}
					return;
				}
			}
			if (ChartName == "县城") {
				for (int i = 0;i < DataGroupButton_sprite_.size();i++) {
					if (IsPressButton(DataGroupButton_sprite_[i], mouse)) {
						if (i == 0) {
							OrderByColumn = "编号";
						}
						else if (i == 2) {
							OrderByColumn = "等级";
						}
						else {
							continue;
						}
						DataBase_.GetOrderByColumn().push_back(OrderByColumn);
						OrderByIsAsc = !OrderByIsAsc;
						DataBase_.GetOrderByIsAsc().push_back(OrderByIsAsc);
						InitDataGroup();
						IsAsc = 1;
						return;
					}
				}
			}
			else{
				if (IsPressButton(DataGroupButton_sprite_[0], mouse)) {
					OrderByColumn = "编号";
					DataBase_.GetOrderByColumn().push_back(OrderByColumn);
					OrderByIsAsc = !OrderByIsAsc;
					DataBase_.GetOrderByIsAsc().push_back(OrderByIsAsc ? "1" : "0");
					InitDataGroup();
					IsAsc = 1;
					return;
				}
			}
		}
	}
}

template<typename T>
void Application::InitDisplayButton(const std::shared_ptr<Player>& player,std::vector<T>& targroup) {
	std::cout << "整理展示函数执行开始-----------------------" << std::endl;
	DisplayButton_.clear();
	std::sort(targroup.begin(), targroup.end(), [](T A, T B) {
		if (A == nullptr || B == nullptr) {
			return false;
		}
		return A->GetID() < B->GetID();
		});
	int ansx = 0, ansy = 0;
	sf::FloatRect firstTextBounds;
	if (!targroup.empty()) {
		sf::Text firstTmp = TextEx_;
		firstTmp.setScale({ 0.9f,0.9f });
		firstTmp.setString(utf8_to_wstring(targroup[0]->GetName()));
		firstTextBounds = firstTmp.getGlobalBounds();
	}

	for (int i = 0; i < targroup.size(); i++) {
		std::wstring str = utf8_to_wstring(targroup[i]->GetName());
		sf::Text temtext = TextEx_;
		temtext.setScale({ 0.9f,0.9f });
		temtext.setString(str);
		temtext.setFillColor(sf::Color::Black);
		float tarx = UseButton_sprite_[0].getPosition().x + ansx * (firstTextBounds.size.x + 10) + 5;
		float tary = UseButton_sprite_[0].getPosition().y + UseButton_[0].getGlobalBounds().size.x / 4 + ansy * (firstTextBounds.size.y + 10) + 5;
		if (ansx < 18) {
			ansx++;
		}
		else {
			ansx = 0;
			ansy++;
		}
		temtext.setPosition({ tarx,tary });
		DisplayButton_.push_back(temtext);
	}
	std::cout << "整理展示函数执行结束" << std::endl;
}

template<typename T>
void Application::DRAWWindow(const sf::Sprite& tarsprite,const std::vector<T>& targroup) {
	int StartX = tarsprite.getPosition().x;
	int EndX = tarsprite.getPosition().x + tarsprite.getGlobalBounds().size.x;
	int StartY = tarsprite.getPosition().y + UseButton_[0].getGlobalBounds().size.y + 2;
	int EndY = tarsprite.getPosition().y + tarsprite.getGlobalBounds().size.y;
	for (int i = 0; i < targroup.size(); i++) {
		if (targroup[i].getPosition().x >= StartX && targroup[i].getPosition().x + targroup[i].getGlobalBounds().size.x <= EndX &&
			targroup[i].getPosition().y >= StartY && targroup[i].getPosition().y + targroup[i].getGlobalBounds().size.y <= EndY) {
			window_.draw(targroup[i]);
		}
	}
}

template<typename T>
void Application::DRAW(const std::vector<T>& tarSpriteGroup) {
	for (int i = 0; i < tarSpriteGroup.size(); i++) {
		window_.draw(tarSpriteGroup[i]);
	}
}

template<typename T>
void Application::DRAW(const std::vector<std::vector<T>>& tarSpriteFroup) {

}

void Application::HandleEvent(const sf::Event::Closed&) {
	window_.close();
	std::cout << "窗口关闭" << std::endl;
}

void Application::HandleEvent(const sf::Event::Resized& resized) {
	std::cout << "窗口长度：" << resized.size.x << " 窗口宽度" << resized.size.y << std::endl;
}

void Application::HandleEvent(const sf::Event::FocusLost&) {
	std::cout << "窗口焦点丢失" << std::endl;
}

void Application::HandleEvent(const sf::Event::FocusGained&) {
	std::cout << "窗口回来了" << std::endl;
}

void Application::HandleEvent(const sf::Event::KeyPressed& key) {
	if (key.code == sf::Keyboard::Key::Escape) {
		window_.close();
	}
	if (key.code == sf::Keyboard::Key::A) {
		TemAddCity();
	}
	else if (key.code == sf::Keyboard::Key::B) {
		TemAddCounty();
	}
	else if (key.code == sf::Keyboard::Key::C) {
		TemAddProvince();
	}
	if ((pd == 3 && pd1 == 308) || pd == 6 || (pd == 5 && pd1 != 5)) {
		if (key.code == sf::Keyboard::Key::Backspace) {
			HelpBackSpace();
		}
		else if (key.code == sf::Keyboard::Key::Enter) {
			if (pd == 3 && pd1 == 308) {
				HelpFindCity();
			}
			else if (pd == 6) {
				HelpCreate();
			}
			else if (pd == 5 && (pd1 != 5)) {
				HelpFind();
			}
		}
	}
	else if (pd == 3 && MainPdButton_[7].getString() != L"当前角色" && temtarmap_ != nullptr) {
		if (key.code == sf::Keyboard::Key::Num1) {
			HelpFight();
		}
	}
}

void Application::HandleEvent(const sf::Event::KeyReleased& key) {

}

void Application::HandleEvent(const sf::Event::MouseButtonPressed& mouse) {
	if (mouse.button == sf::Mouse::Button::Left) {
		if (pd == 0) {
			HelpPd0(mouse);
		}
		else if (pd == 1) {
			HelpPd1(mouse);
		}
		else if (pd == 2) {
			HelpPd2(mouse);
		}
		else if (pd == 3) {
			HelpPd3(mouse);
		}
		else if (pd == 4) {
			HelpPd4(mouse);
		}
		else if (pd == 5) {
			HelpPd5(mouse);
		}
		else if (pd == 6) {
			HelpPd6(mouse);
		}
		else if (pd == 7) {
			HelpPd7(mouse);
		}
	}
}

void Application::HandleEvent(const sf::Event::MouseButtonReleased& mouse) {
	std::cout << "鼠标松开了" << std::endl;
	IsDraging = 0;
	StartPosition = { 0,0 };
}

void Application::HandleEvent(const sf::Event::MouseWheelScrolled& mouse) {
	if (pd == 3) {
		if (mouse.delta > 0) {
			PY -= 6;
			if (PY < 10 || PY>120) {
				PY += 6;
			}
			if (PreY + PY / 2 >= Map_.size() || PreX + PY >= Map_[0].size() || PreX < 0 || PreY < 0) {
				PY += 6;
			}
			UpdateMap(PreX, PreX + PY, PreY, PreY + PY / 2);
			if (PY == PYB) {
				Change(TarMap[0]);
			}
		}
		else if (mouse.delta < 0) {
			PY += 6;
			if (PY < 10 || PY>120) {
				PY -= 6;
			}
			if (PreY + PY / 2 >= Map_.size() || PreX + PY >= Map_[0].size() || PreX < 0 || PreY < 0) {
				PY -= 6;
			}
			UpdateMap(PreX, PreX + PY, PreY, PreY + PY / 2);
			if (PY == PYB) {
				Change(TarMap[0]);
			}
		}
	}
	else if (pd == 5 && pd1 == 5) {
		if (mouse.delta < 0) {
			if (DisplayButton_[DisplayButton_.size() - 1].getPosition().y + DisplayButton_[DisplayButton_.size() - 1].getGlobalBounds().size.y
		> UseButton_sprite_[0].getPosition().y + UseButton_sprite_[0].getGlobalBounds().size.y - 20) {
				for (int i = 0; i < DisplayButton_.size(); i++) {
					float tarx = DisplayButton_[i].getPosition().x;
					float tary = DisplayButton_[i].getPosition().y;
					DisplayButton_[i].setPosition({ tarx,tary - 15 });
				}
			}
		}
		else if (mouse.delta > 0) {
			if (DisplayButton_[0].getPosition().y < UseButton_sprite_[0].getPosition().y + UseButton_[0].getGlobalBounds().size.x / 4 + 5) {
				for (int i = 0; i < DisplayButton_.size(); i++) {
					float tarx = DisplayButton_[i].getPosition().x;
					float tary = DisplayButton_[i].getPosition().y;
					DisplayButton_[i].setPosition({ tarx,tary + 15 });
				}
			}
		}
	}
}

void Application::HandleEvent(const sf::Event::MouseMoved& mouse) {
	if (pd == 3) {
		sf::Sprite& temsprite = MainPdButton_sprite_[10];
		sf::Sprite& temsprite1 = MainPdButton_sprite_[7];
		sf::Sprite& temsprite2 = MainPdButton_sprite_[11];
		//展示角色列表
		if (mouse.position.x >= temsprite1.getPosition().x && mouse.position.x <= temsprite1.getPosition().x + temsprite1.getGlobalBounds().size.x) {
			if (pd1 % 100 != 7 && mouse.position.y >= temsprite1.getPosition().y && mouse.position.y <= temsprite1.getPosition().y + temsprite1.getGlobalBounds().size.y) {
				if (PlayerGroupButton_sprite_.size() != PlayerGroup_.size()) {
					InitPlayerGroupButton(temsprite1.getPosition().x, temsprite1.getPosition().x + temsprite1.getGlobalBounds().size.x, temsprite1.getPosition().y, temsprite1.getPosition().y + temsprite1.getGlobalBounds().size.y);
				}
				if (pd1 % 10000 / 100 != 7) {
					pd1 = pd1 * 10000 + 700;
				}
			}
			else if (pd1 % 10000 / 100 == 7 && mouse.position.y >= temsprite.getPosition().y && mouse.position.y <= temsprite.getPosition().y + temsprite.getGlobalBounds().size.y * (PlayerGroupButton_sprite_.size() + 1)) {

			}
			else {
				if (pd1 > 10000) {
					pd1 /= 10000;
				}
			}
		}
		//展示事件类型列表
		else if (MainPdButton_[7].getString() != L"当前角色" && mouse.position.x >= temsprite.getPosition().x && mouse.position.x <= temsprite.getPosition().x + temsprite.getGlobalBounds().size.x) {
			if (pd1 % 100 != 10 && mouse.position.y >= temsprite.getPosition().y && mouse.position.y <= temsprite.getPosition().y + temsprite.getGlobalBounds().size.y) {
				if (ThingTypeButton_sprite_.size() == 0) {
					InitChooseThingTypeButton(temsprite.getPosition().x, temsprite.getPosition().x + temsprite.getGlobalBounds().size.x, temsprite.getPosition().y, temsprite.getPosition().y + temsprite.getGlobalBounds().size.y);
				}
				if (pd1 % 10000 / 100 != 10) {
					pd1 = pd1 * 10000 + 1000;
				}
			}
			else if (pd1 % 10000 / 100 == 10 && mouse.position.y >= temsprite.getPosition().y && mouse.position.y <= temsprite.getPosition().y + temsprite.getGlobalBounds().size.y * (ThingTypeButton_sprite_.size() + 1)) {
				if (mouse.position.y >= ThingTypeButton_sprite_[1].getPosition().y && mouse.position.y <= ThingTypeButton_sprite_[1].getPosition().y + ThingTypeButton_sprite_[1].getGlobalBounds().size.y) {
					if (BuildGroupButton_sprite_.size() == 0) {
						InitBuildGroupButton(ThingTypeButton_sprite_[1].getPosition().x, ThingTypeButton_sprite_[1].getPosition().x + ThingTypeButton_sprite_[1].getGlobalBounds().size.x, ThingTypeButton_sprite_[1].getPosition().y, ThingTypeButton_sprite_[1].getPosition().y + ThingTypeButton_sprite_[1].getGlobalBounds().size.y);
					}
					if (pd1 % 100 != 1) {
						pd1 = pd1 / 100 * 100 + 1;
					}
				}
				else {
					pd1 = pd1 / 100 * 100;
				}
			}
			else {
				if (pd1 >= 10000) {
					pd1 /= 10000;
				}
			}
		}
		//展示建设消耗列表
		else if (pd1 % 10000 == 1001) {
			if (mouse.position.x >= BuildGroupButton_sprite_[0].getPosition().x && mouse.position.x <= BuildGroupButton_sprite_[0].getPosition().x + BuildGroupButton_sprite_[0].getGlobalBounds().size.x &&
				mouse.position.y >= BuildGroupButton_sprite_[0].getPosition().y && mouse.position.y <= BuildGroupButton_sprite_[0].getPosition().y + BuildGroupButton_sprite_[0].getGlobalBounds().size.y) {
				if (pd5 == 0) {
					InitLoseDisplayButton(BuildGroupButton_sprite_[0].getPosition().x, BuildGroupButton_sprite_[0].getPosition().x + BuildGroupButton_sprite_[0].getGlobalBounds().size.x, BuildGroupButton_sprite_[0].getPosition().y, BuildGroupButton_sprite_[0].getPosition().y + BuildGroupButton_sprite_[0].getGlobalBounds().size.y, BuildGroup_[0]);
				}
				pd5 = 0;
			}
			else if (mouse.position.x >= BuildGroupButton_sprite_[1].getPosition().x && mouse.position.x <= BuildGroupButton_sprite_[1].getPosition().x + BuildGroupButton_sprite_[1].getGlobalBounds().size.x &&
				mouse.position.y >= BuildGroupButton_sprite_[1].getPosition().y && mouse.position.y <= BuildGroupButton_sprite_[1].getPosition().y + BuildGroupButton_sprite_[1].getGlobalBounds().size.y) {
				if (pd5 == 1) {
					InitLoseDisplayButton(BuildGroupButton_sprite_[1].getPosition().x, BuildGroupButton_sprite_[1].getPosition().x + BuildGroupButton_sprite_[1].getGlobalBounds().size.x, BuildGroupButton_sprite_[1].getPosition().y, BuildGroupButton_sprite_[1].getPosition().y + BuildGroupButton_sprite_[1].getGlobalBounds().size.y, BuildGroup_[1]);
				}
				pd5 = 1;
			}
			else {
				pd5 = -1;
				pd1 = pd1 / 100 * 100;
			}
		}
		//展示资源列表
		else if (MainPdButton_[7].getString() != L"当前角色" && mouse.position.x >= temsprite2.getPosition().x && mouse.position.x <= temsprite2.getPosition().x + temsprite2.getGlobalBounds().size.x) {
			if (pd1 % 100 != 11 && mouse.position.y >= temsprite2.getPosition().y && mouse.position.y <= temsprite2.getPosition().y + temsprite2.getGlobalBounds().size.y) {
				if (pd1 % 10000 / 100 != 11) {
					InitResourceButton(temsprite2.getPosition().x, temsprite2.getPosition().x + temsprite2.getGlobalBounds().size.x, temsprite2.getPosition().y, temsprite2.getPosition().y + temsprite2.getGlobalBounds().size.y);
				}
				if (pd1 % 10000 / 100 != 11) {
					pd1 = pd1 * 10000 + 1100;
				}
			}
			else if (pd1 % 10000 / 100 == 11 && mouse.position.y >= temsprite2.getPosition().y && mouse.position.y <= temsprite2.getPosition().y + temsprite2.getGlobalBounds().size.y * (ResourceButton_sprite_.size() + 1)) {

			}
			else {
				if (pd1 > 10000) {
					pd1 /= 10000;
				}
			}
		}
		//其他
		else {
			if (pd1 >= 10000) {
				pd1 /= 10000;
			}
		}
	}
}

void Application::HandleEvent(const sf::Event::TextEntered& textEvent) {
	if ((pd == 3 && pd1 == 308) || pd == 6 || (pd == 5 && pd1 != 5)) {
		if (textEvent.unicode >= 32) {
			TarDialog += static_cast<wchar_t>(textEvent.unicode);
		}
	}
}

void Application::TemAddCity() {
	if (PlayerGroup_.size() == 0) {
		return;
	}
	for (int i = 0; i < CityGroup_.size(); i++) {
		if (!CityGroup_[i]->IsBelong()) {
			CityGroup_[i]->SetBelongPlayer(PlayerGroup_[0]);
			PlayerGroup_[0]->GetCityGroup().push_back(CityGroup_[i]);
		}
	}
}

void Application::TemAddCounty() {
	if (PlayerGroup_.size() == 0) {
		return;
	}
	for (int i = 0; i < CountyGroup_.size(); i++) {
		if (!CountyGroup_[i]->IsBelong()) {
			CountyGroup_[i]->SetBelongPlayer(PlayerGroup_[0]);
			PlayerGroup_[0]->GetCountyGroup().push_back(CountyGroup_[i]);
		}
	}
}

void Application::TemAddProvince() {
	if (PlayerGroup_.size() == 0) {
		return;
	}
	for (int i = 0; i < ProvinceGroup_.size(); i++) {
		if (!ProvinceGroup_[i]->IsBelong()) {
			ProvinceGroup_[i]->SetBelongPlayer(PlayerGroup_[0]);
			PlayerGroup_[0]->GetProvinceGroup().push_back(ProvinceGroup_[i]);
		}
	}
}
