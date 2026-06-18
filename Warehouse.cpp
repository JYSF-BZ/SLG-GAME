#include "Warehouse.h"
namespace fs = std::filesystem;

WareHouse WareHouse::instance;

void WareHouse::CutPicture(const sf::Texture& tartexture, int num, int site) {
	std::cout << "切割图像函数执行开始-----------------------" << std::endl;
	if (tartexture.getSize().x == 0 || tartexture.getSize().y == 0) {
		std::cout << "切割图像函数因图像不符合标准执行结束" << std::endl;
		return;
	}
	if (num <= 0) {
		std::cout << "切割图像函数因切割数量不正确执行结束" << std::endl;
		return;
	}
	int cols = static_cast<int>(std::sqrt(num));
	int rows = num / cols;
	if (cols * rows < num) {
		rows++;
	}
	if (cols == 0) {
		cols = 1;
		rows = 1;
	}
	float textureWidth = static_cast<float>(tartexture.getSize().x);
	float textureHeight = static_cast<float>(tartexture.getSize().y);
	float blockWidth = textureWidth / cols;
	float blockHeight = textureHeight / rows;
	int blockCount = 0;
	sf::Image tarimage = tartexture.copyToImage();
	for (int r = 0; r < rows && blockCount < num; r++) {
		for (int c = 0; c < cols && blockCount < num; c++) {
			sf::Vector2i pos(static_cast<int>(c * blockWidth), static_cast<int>(r * blockHeight));
			sf::Vector2i size(static_cast<int>(blockWidth), static_cast<int>(blockHeight));
			sf::IntRect blockRect(pos, size);
			sf::Texture temtexture(tarimage, 0, blockRect);
			LandMod[site].push_back(temtexture);
			blockCount++;
		}
	}
	std::cout << "切割图像函数执行结束" << std::endl;
}

std::wstring WareHouse::utf8_to_wstring(const std::string& utf8)const{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	std::wstring wstr(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], len);
	return wstr;
}

void WareHouse::InitDataBase() {
	std::cout << "初始化库创建函数执行开始-----------------------" << std::endl;

	DataBase_.ShowDataBase("城池库");
	if (DataBase_.IsHaveData()) {
		IsExistDataBase = 1;
	}
	else {
		DataBase_.CreateDataBase("城池库");
	}

	DataBase_.UseTest("城池库");

	if (!IsExistDataBase) {
		DataBase_.GetColumnName() = { "编号","归属州","归属郡","县","县令","地位","等级","坐标" };
		DataBase_.GetColumnType() = { "int","varchar","varchar","varchar","varchar","varchar","int","varchar" };
		DataBase_.GetColumnB() = { 0,20,20,20,20,20,0,20 };
		DataBase_.GetColumnIsPrimaryKey() = { 1,0,0,0,0,0,0,0 };
		DataBase_.GetColumnIsNotNull() = { 1,1,1,1,0,1,1,1 };
		DataBase_.GetColumnIsUnique() = { 1,0,0,1,0,0,0,1 };
		DataBase_.GetColumnIsAuto_increasement() = { 0,0,0,0,0,0,0,0 };
		DataBase_.CreateTable("县城");

		DataBase_.GetColumnName() = { "编号","归属州","郡","太守" };
		DataBase_.GetColumnType() = { "int","varchar","varchar","varchar" };
		DataBase_.GetColumnB() = { 0,20,20,20 };
		DataBase_.GetColumnIsPrimaryKey() = { 1,0,0,0 };
		DataBase_.GetColumnIsNotNull() = { 1,1,1,0 };
		DataBase_.GetColumnIsUnique() = { 1,0,1,0 };
		DataBase_.GetColumnIsAuto_increasement() = { 0,0,0,0 };
		DataBase_.CreateTable("郡");

		DataBase_.GetColumnName() = { "编号","州","刺史" };
		DataBase_.GetColumnType() = { "int","varchar","varchar" };
		DataBase_.GetColumnB() = { 0,20,20 };
		DataBase_.GetColumnIsPrimaryKey() = { 1,0,0 };
		DataBase_.GetColumnIsNotNull() = { 1,1,0 };
		DataBase_.GetColumnIsUnique() = { 1,1,0 };
		DataBase_.GetColumnIsAuto_increasement() = { 0,0,0 };
		DataBase_.CreateTable("州");
	}

	std::cout << "初始化库创建函数执行结束" << std::endl;
}

void WareHouse::InitFont(){
	std::cout << "初始化字体函数执行开始-----------------------" << std::endl;
	FontGroup.emplace_back(L"Resource/字体/STXINGKA.TTF");
	std::cout << "初始化字体函数执行结束" << std::endl;
}

void WareHouse::InitTexture(){
	std::cout << "初始化纹理函数执行开始-----------------------" << std::endl;
	Inittexture(L"Resource/图片/background.png");
	Inittexture(L"Resource/图片/StartGame.png");
	Inittexture(L"Resource/图片/Mainback.png");
	Inittexture(L"Resource/图片/Button.png");
	Inittexture(L"Resource/图片/Mapback.png");
	std::cout << "初始化纹理函数执行结束" << std::endl;
}

void WareHouse::Inittexture(const std::wstring& FilePath){
	TextureGroup.emplace_back(FilePath);
	TextureGroup[TextureGroup.size() - 1].setSmooth(true);
}

void WareHouse::InitSprite(){
	std::cout << "初始化精灵函数执行开始-----------------------" << std::endl;
	//1.总背景,2.开始按钮,3.主背景,4.按钮范例,5.Geo背景;
	for (int i = 0;i < TextureGroup.size();i++) {
		SpriteGroup.emplace_back(TextureGroup[i]);
	}
	std::cout << "初始化精灵函数执行结束" << std::endl;
}

void WareHouse::InitChunk(){
	std::cout << "初始化Chunk函数执行开始-----------------------" << std::endl;
	int PHMax = Map.size(), PWMax = Map[0].size();
	int PWNum = (PWMax + 63) / 64, PHNum = (PHMax + 63) / 64;
	int ChunkNum = PWNum * PHNum;
	MyChunk.resize(ChunkNum);
	MyCChunk.resize(ChunkNum);
	for (int i = 0;i < Map.size();i++) {
		for (int j = 0;j < Map[i].size();j++) {
			if (!Map[i][j]->IsNull()) {
				int index = i / 64 * PWNum + j / 64;
				MyChunk[index].JoinGrid(i % 64, j % 64, Map[i][j]);
				MyCChunk[index].JoinGrid(i % 64, j % 64, Map[i][j]);
				Map[i][j]->GetBelongCity()->GetChunkID().insert(index);
				Map[i][j]->GetBelongCounty()->GetChunkID().insert(index);
				Map[i][j]->GetBelongProvince()->GetChunkID().insert(index);
			}
		}
	}
	for (auto& city : CityGroup) {
		city->SetFourBorderID(PWNum);
	}
	for (auto& county : CountyGroup) {
		county->SetFourBorderID(PWNum);
	}
	for (auto& province : ProvinceGroup) {
		province->SetFourBorderID(PWNum);
	}
	std::cout << "初始化Chunk函数执行结束" << std::endl;
	return;
}

void WareHouse::InitIsColorGroup(){
	IsColorGroup.resize(ColorGroupSize, false);
}

void WareHouse::ADDLANDMOD() {
	std::wstring tem[4] = { L"粮草",L"石料",L"木材",L"铁矿" };
	std::wstring tem1[4] = { L"二级",L"三级",L"四级",L"五级" };
	LandMod.resize(10);
	for (int i = 0; i < LandMod.size(); i++) {
		if (i != 0 && i < 5) {
			for (int j = 0; j < 4; j++) {
				std::wstring path = L"Resource/图片/" + tem1[i - 1] + tem[j] + L"土地.png";
				sf::Texture temtexture(path);
				temtexture.setSmooth(true);
				LandMod[i].emplace_back(temtexture);
				sf::Sprite temsprite(LandMod[i].back());
			}
		}
		else if (i == 0) {
			std::wstring tarPath = L"";
			for (int j = 0; j < 5; j++) {
				if (j == 0) {
					tarPath = L"Resource/图片/一级土地.png";
				}
				else if (j == 1) {
					tarPath = L"Resource/图片/河流.png";
				}
				else if (j == 2) {
					tarPath = L"Resource/图片/要塞.png";
				}
				else if (j == 3) {
					tarPath = L"Resource/图片/桥梁.png";
				}
				else if (j == 4) {
					tarPath = L"Resource/图片/云朵.png";
				}
				sf::Texture temtexture(tarPath);
				temtexture.setSmooth(true);
				LandMod[i].emplace_back(temtexture);
				sf::Sprite temsprite(LandMod[i].back());
			}
		}
		else if (i == 5) {
			std::wstring tarPath = L"";
			for (int j = 0;j < 2;j++) {
				if (j == 0) {
					tarPath = L"Resource/图片/小型山脉.png";
				}
				else if (j == 1) {
					tarPath = L"Resource/图片/中型山脉.png";
				}
				sf::Texture temtexture(tarPath);
				temtexture.setSmooth(true);
				LandMod[i].emplace_back(temtexture);
				sf::Sprite temsprite(LandMod[i].back());
			}
			CutPicture(LandMod[5][1], 4, i);
		}
		else if (i == 6) {
			std::wstring tarPath = L"Resource/图片/大型山脉.png";;
			sf::Texture temtexture(tarPath);
			temtexture.setSmooth(true);
			LandMod[i].emplace_back(temtexture);
			sf::Sprite temsprite(LandMod[i].back());
			CutPicture(LandMod[6][0], 9, i);
		}
		else if (i == 7) {
			std::wstring tarPath = L"";
			for (int j = 0;j < 2;j++) {
				if (j == 0) {
					tarPath = L"Resource/图片/小型深山.png";
				}
				else if (j == 1) {
					tarPath = L"Resource/图片/中型深山.png";
				}
				sf::Texture temtexture(tarPath);
				temtexture.setSmooth(true);
				LandMod[i].emplace_back(temtexture);
				sf::Sprite temsprite(LandMod[i].back());
			}
			CutPicture(LandMod[7][1], 4, i);
		}
		else if (i == 8) {
			std::wstring tarPath = L"Resource/图片/大型深山.png";
			sf::Texture temtexture(tarPath);
			temtexture.setSmooth(true);
			LandMod[i].emplace_back(temtexture);
			sf::Sprite temsprite(LandMod[i].back());
			CutPicture(LandMod[8][0], 9, i);
		}
		else if (i == 9) {
			sf::Texture temtexture(L"Resource/图片/城池.png");
			temtexture.setSmooth(true);
			LandMod[i].emplace_back(temtexture);
			sf::Sprite temsprite(LandMod[i].back());
			CutPicture(LandMod[9][0], 25, i);
			CutPicture(LandMod[9][0], 81, i);
			CutPicture(LandMod[9][0], 169, i);
		}
	}
}

void WareHouse::ADDCITY() {
	//加入县级
	std::ifstream file(utf8_to_wstring("Resource/县级库总览.txt"));
	std::string line;
	if (!IsExistDataBase) {
		DataBase_.GetTitle() = { "编号","归属州","归属郡","县","地位","坐标","等级" };
	}
	int ans = 0;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		std::istringstream lineStream(line);
		std::string ProName;
		std::string Name;
		lineStream >> ProName >> Name;
		AddCity(ProName, Name, ans);
		ans++;
	}
	if (!IsExistDataBase) {
		DataBase_.InsertRowDataIgnore().InsertRowData("县城");
	}
	file.close();
	CityGroupSize = CityGroup.size();
}

void WareHouse::ADDCOUNTY() {
	//加入市级
	std::ifstream file(utf8_to_wstring("Resource/郡级库总览.txt"));
	std::string line;
	if (!IsExistDataBase) {
		DataBase_.GetTitle() = { "编号","归属州","郡" };
	}
	int ans = 0;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		std::istringstream lineStream(line);
		std::string ProName;
		std::string Name;
		lineStream >> ProName >> Name;
		AddCounty(ProName, Name, ans);
		ans++;
	}
	file.close();
	if (!IsExistDataBase) {
		DataBase_.InsertRowDataIgnore().InsertRowData("郡");
	}
	CountyGroupSize = CountyGroup.size();
}

void WareHouse::ADDPROVINCE() {
	//加入省级
	std::ifstream file(utf8_to_wstring("Resource/州级库总览.txt"));
	std::string line;
	if (!IsExistDataBase) {
		DataBase_.GetTitle() = { "编号","州" };
	}
	int ans = 0;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		std::istringstream lineStream(line);
		std::string Name;
		lineStream >> Name;
		AddProvince(Name, ans);
		ans++;
	}
	if (!IsExistDataBase) {
		DataBase_.InsertRowDataIgnore().InsertRowData("州");
	}
	file.close();
	ProvinceGroupSize = ProvinceGroup.size();
}

void WareHouse::ADDMAP() {
	for (auto& tarprovince : ProvinceGroup) {
		if (tarprovince->GetOnlyMapSize().Hmin < HMIN) {
			HMIN = tarprovince->GetOnlyMapSize().Hmin;
		}
		if (tarprovince->GetOnlyMapSize().Wmin < WMIN) {
			WMIN = tarprovince->GetOnlyMapSize().Wmin;
		}
		if (tarprovince->GetOnlyMapSize().Hmax > HMAX) {
			HMAX = tarprovince->GetOnlyMapSize().Hmax;
		}
		if (tarprovince->GetOnlyMapSize().Wmax > WMAX) {
			WMAX = tarprovince->GetOnlyMapSize().Wmax;
		}
	}
	Map.resize(HMAX - HMIN + 1);
	for (int i = 0; i < Map.size(); i++) {
		Map[i].resize(WMAX - WMIN + 1);
	}
	for (auto& tarcity : CityGroup) {
		std::vector<std::shared_ptr<Grid>>& citygrid = tarcity->GetCityMap();
		for (auto& targrid : citygrid) {
			Map[targrid->GetGrid().getPosition().y - HMIN][targrid->GetGrid().getPosition().x - WMIN] = targrid;
		}
	}
	for (int i = 0;i < Map.size();i++) {
		for (int j = 0;j < Map[i].size();j++) {
			if (Map[i][j] == nullptr) {
				Map[i][j] = std::make_shared<Grid>();
				Map[i][j]->SetNull(i + HMIN, j + WMIN);
			}
		}
	}
	AddMap();
}

void WareHouse::ADDBUILD() {
	//加入建筑物集
	std::ifstream file(utf8_to_wstring("Resource/建筑物消耗.txt"));
	std::string line;
	int ans = 0;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		std::istringstream lineStream(line);
		std::string Name;
		int losegrain, loseiron, losetimber, losestone, losemoney;
		lineStream >> Name >> losegrain >> loseiron >> losetimber >> losestone >> losemoney;
		Build build(Name, losegrain, loseiron, losetimber, losestone, losemoney, ans);
		BuildGroup.push_back(build);
		ans++;
	}
	file.close();
	BuildGroupSize = BuildGroup.size();
}

void WareHouse::ADDCOLOR() {
	ColorGroup.push_back(sf::Color::Red);
	ColorGroup.push_back(sf::Color::Cyan);
	ColorGroup.push_back(sf::Color(204, 0, 204));
	ColorGroup.push_back(sf::Color::Green);
	ColorGroupSize = ColorGroup.size();
	InitIsColorGroup();
}

void WareHouse::AddCity(const std::string& ProvinceName, const std::string& Name, int id) {
	auto city = std::make_shared<City>(utf8_to_wstring("Resource/县级库/" + ProvinceName + Name + ".txt"), IsExistDataBase);
	city->Setid(id);
	city->JoinAllGrid();
	CityGroup.push_back(city);
}

void WareHouse::AddCounty(const std::string& ProvinceName, const std::string& Name, int id) {
	auto county = std::make_shared<County>(utf8_to_wstring("Resource/郡级库/" + ProvinceName  + Name + "郡.txt"), IsExistDataBase);
	county->Setid(id);
	county->JoinAllCity();
	CountyGroup.push_back(county);
}

void WareHouse::AddProvince(const std::string& ProvinceName, int id) {
	auto province = std::make_shared<Province>(utf8_to_wstring("Resource/州级库/" + ProvinceName + ".txt"), IsExistDataBase);
	province->Setid(id);
	province->JoinAllCounty();
	province->JoinAllCity();
	ProvinceGroup.push_back(province);
}

void WareHouse::AddMap() {
	std::string tem[5] = { "粮草","石料","木材","铁矿","土地" };
	std::string tem1[5] = { "一级","二级","三级","四级","五级" };
	for (int i = 0; i < Map.size(); i++) {
		for (int j = 0; j < Map[i].size(); j++) {
			if (Map[i][j]==nullptr) {
				continue;
			}
			else {
				if (!Map[i][j]->IsNull()) {
					int temcityid = Map[i][j]->GetBelongCity()->GetID();
					int temcountyid = Map[i][j]->GetBelongCounty()->GetID();
					if (i - 1 >= 0 &&Map[i - 1][j] != nullptr) {
						if (Map[i - 1][j]->IsNull()) {
							Map[i][j]->SetUp(0);
						}
						else {
							if (Map[i - 1][j]->GetBelongCity()->GetID() != temcityid) {
								Map[i][j]->SetUp();
							}
							if (Map[i - 1][j]->GetBelongCounty()->GetID() != temcountyid) {
								Map[i][j]->SetUp(0);
							}
						}	
					}
					else {
						Map[i][j]->SetUp(0);
					}
					if (i + 1 < Map.size() && Map[i + 1][j] != nullptr) {
						if (Map[i + 1][j]->IsNull()) {
							Map[i][j]->SetDown(0);
						}
						else {
							if (Map[i + 1][j]->GetBelongCity()->GetID() != temcityid) {
								Map[i][j]->SetDown();
							}
							if (Map[i + 1][j]->GetBelongCounty()->GetID() != temcountyid) {
								Map[i][j]->SetDown(0);
							}
						}
					}
					else {
						Map[i][j]->SetDown(0);
					}
					if (j - 1 >= 0 && Map[i][j - 1] != nullptr) {
						if (Map[i][j - 1]->IsNull()) {

							Map[i][j]->SetLeft(0);
						}
						else {
							if (Map[i][j - 1]->GetBelongCity()->GetID() != temcityid) {
								Map[i][j]->SetLeft();
							}
							if (Map[i][j - 1]->GetBelongCounty()->GetID() != temcountyid) {
								Map[i][j]->SetLeft(0);
							}
						}
					}
					else {
						Map[i][j]->SetLeft(0);
					}
					if (j + 1 < Map[0].size() && Map[i][j + 1] != nullptr) {
						if (Map[i][j + 1]->IsNull()) {
							Map[i][j]->SetRight(0);
						}
						else {
							if (Map[i][j + 1]->GetBelongCity()->GetID() != temcityid) {
								Map[i][j]->SetRight();
							}
							if (Map[i][j + 1]->GetBelongCounty()->GetID() != temcountyid) {
								Map[i][j]->SetRight(0);
							}
						}
					}
					else {
						Map[i][j]->SetRight(0);
					}
				}
				if (Map[i][j]->IsSprite()) {
					continue;
				}
			}
			if (Map[i][j]->GetSite() == "农田") {
				int level = rand() % 100;
				int levelx = 0, levely = 0;
				int level1 = rand() % 4;
				if (level < 25) {
					levelx = 0;
				}
				else if (level < 55) {
					levelx = 1;
				}
				else if (level < 80) {
					levelx = 2;
				}
				else if (level < 95) {
					levelx = 3;
				}
				else {
					levelx = 4;
				}
				if (level < 25) {
					levely = 4;
				}
				else {
					levely = level1;
				}
				if (levelx == 0) {
					Map[i][j]->SetSprite(LandMod[levelx][0]);
					Map[i][j]->Getjsonimagenum().push_back(0);
				}
				else {
					Map[i][j]->SetSprite(LandMod[levelx][levely]);
					Map[i][j]->Getjsonimagenum().push_back(levelx * 10 + levely);
				}
				Map[i][j]->SetSite(tem1[levelx] + tem[levely]);
				Map[i][j]->SetResource(levelx);
			}
			else if (Map[i][j]->GetSite() == "河流") {
				Map[i][j]->SetSprite(LandMod[0][1]);
				Map[i][j]->Getjsonimagenum().push_back(1);
			}
			else if (Map[i][j]->GetSite() == "山脉") {
				bool tempd = 0, tempd1 = 0;
				if (i + 1 < Map.size() && j + 1 < Map[0].size()) {
					if (!Map[i + 1][j]->IsNull() && !Map[i][j + 1]->IsNull() && !Map[i + 1][j + 1]->IsNull() &&
						Map[i + 1][j]->GetSite() == "山脉" && Map[i][j + 1]->GetSite() == "山脉" && Map[i + 1][j + 1]->GetSite() == "山脉" &&
						!Map[i + 1][j]->IsSprite() && !Map[i][j + 1]->IsSprite() && !Map[i + 1][j + 1]->IsSprite()) {
						tempd = 1;
					}
				}
				if (tempd&&i + 2 < Map.size() && j + 2 < Map[0].size()) {
					if (!Map[i + 2][j]->IsNull() && !Map[i + 2][j + 1]->IsNull() && !Map[i + 2][j + 2]->IsNull() && !Map[i + 1][j + 2]->IsNull() && !Map[i][j + 2]->IsNull() &&
						Map[i + 2][j]->GetSite() == "山脉" && Map[i + 2][j + 1]->GetSite() == "山脉" && Map[i + 2][j + 2]->GetSite() == "山脉" && Map[i + 1][j + 2]->GetSite() == "山脉" && Map[i][j + 2]->GetSite() == "山脉" &&
						!Map[i + 2][j]->IsSprite() && !Map[i + 2][j + 1]->IsSprite() && !Map[i + 2][j + 2]->IsSprite() && !Map[i + 1][j + 2]->IsSprite() && !Map[i][j + 2]->IsSprite()) {
						tempd1 = 1;
					}
				}
				if (tempd && tempd1) {
					int random = rand() % 10;
					if (random < 7) {
						int temjsonnum = 61;
						for (int ii = 0;ii < 3;ii++) {
							for (int jj = 0;jj < 3;jj++) {
								Map[i + ii][j + jj]->SetSprite(LandMod[6][temjsonnum % 10]);
								Map[i + ii][j + jj]->Getjsonimagenum().push_back(temjsonnum);
								temjsonnum++;
							}
						}
					}
					else if (random < 9) {
						int temjsonnum = 52;
						for (int ii = 0;ii < 2;ii++) {
							for (int jj = 0;jj < 2;jj++) {
								Map[i + ii][j + jj]->SetSprite(LandMod[5][temjsonnum % 10]);
								Map[i + ii][j + jj]->Getjsonimagenum().push_back(temjsonnum);
								temjsonnum++;
							}
						}
					}
					else {
						Map[i][j]->SetSprite(LandMod[5][0]);
						Map[i][j]->Getjsonimagenum().push_back(50);
					}
				}
				else if (tempd && !tempd1) {
					int random = rand() % 10;
					if (random < 7) {
						int temjsonnum = 52;
						for (int ii = 0;ii < 2;ii++) {
							for (int jj = 0;jj < 2;jj++) {
								Map[i + ii][j + jj]->SetSprite(LandMod[5][temjsonnum % 10]);
								Map[i + ii][j + jj]->Getjsonimagenum().push_back(temjsonnum);
								temjsonnum++;
							}
						}
					}
					else {
						Map[i][j]->SetSprite(LandMod[5][0]);
						Map[i][j]->Getjsonimagenum().push_back(50);
					}
				}
				else {
					Map[i][j]->SetSprite(LandMod[5][0]);
					Map[i][j]->Getjsonimagenum().push_back(50);
				}
			}
			else if (Map[i][j]->GetSite() == "深山") {
				bool tempd = 0, tempd1 = 0;
				if (i + 1 < Map.size() && j + 1 < Map[0].size()) {
					if (!Map[i + 1][j]->IsNull() && !Map[i][j + 1]->IsNull() && !Map[i + 1][j + 1]->IsNull() &&
						Map[i + 1][j]->GetSite() == "深山" && Map[i][j + 1]->GetSite() == "深山" && Map[i + 1][j + 1]->GetSite() == "深山" &&
						!Map[i + 1][j]->IsSprite() && !Map[i][j + 1]->IsSprite() && !Map[i + 1][j + 1]->IsSprite()) {
						tempd = 1;
					}
				}
				if (tempd&& i + 2 < Map.size() && j + 2 < Map[0].size()) {
					if (!Map[i + 2][j]->IsNull() && !Map[i + 2][j + 1]->IsNull() && !Map[i + 2][j + 2]->IsNull() && !Map[i + 1][j + 2]->IsNull() && !Map[i][j + 2]->IsNull() &&
						Map[i + 2][j]->GetSite() == "深山" && Map[i + 2][j + 1]->GetSite() == "深山" && Map[i + 2][j + 2]->GetSite() == "深山" && Map[i + 1][j + 2]->GetSite() == "深山" && Map[i][j + 2]->GetSite() == "深山" &&
						!Map[i + 2][j]->IsSprite() && !Map[i + 2][j + 1]->IsSprite() && !Map[i + 2][j + 2]->IsSprite() && !Map[i + 1][j + 2]->IsSprite() && !Map[i][j + 2]->IsSprite()) {
						tempd1 = 1;
					}
				}
				if (tempd && tempd1) {
					int random = rand() % 10;
					if (random < 7) {
						int temjsonnum = 81;
						for (int ii = 0;ii < 3;ii++) {
							for (int jj = 0;jj < 3;jj++) {
								Map[i + ii][j + jj]->SetSprite(LandMod[8][temjsonnum % 10]);
								Map[i + ii][j + jj]->Getjsonimagenum().push_back(temjsonnum);
								temjsonnum++;
							}
						}
					}
					else if (random < 9) {
						int temjsonnum = 72;
						for (int ii = 0;ii < 2;ii++) {
							for (int jj = 0;jj < 2;jj++) {
								Map[i + ii][j + jj]->SetSprite(LandMod[7][temjsonnum % 10]);
								Map[i + ii][j + jj]->Getjsonimagenum().push_back(temjsonnum);
								temjsonnum++;
							}
						}
					}
					else {
						Map[i][j]->SetSprite(LandMod[7][0]);
						Map[i][j]->Getjsonimagenum().push_back(70);
					}
				}
				else if (tempd && !tempd1) {
					int random = rand() % 10;
					if (random < 7) {
						int temjsonnum = 72;
						for (int ii = 0;ii < 2;ii++) {
							for (int jj = 0;jj < 2;jj++) {
								Map[i + ii][j + jj]->SetSprite(LandMod[7][temjsonnum % 10]);
								Map[i + ii][j + jj]->Getjsonimagenum().push_back(temjsonnum);
								temjsonnum++;
							}
						}
					}
					else {
						Map[i][j]->SetSprite(LandMod[7][0]);
						Map[i][j]->Getjsonimagenum().push_back(70);
					}
				}
				else {
					Map[i][j]->SetSprite(LandMod[7][0]);
					Map[i][j]->Getjsonimagenum().push_back(70);
				}
			}
			else if (Map[i][j]->GetSite() == "城墙") {
				if (i + 13 < Map.size() && j + 13 < Map[0].size() && Map[i + 3][j + 3]->GetSite() == "市井" && Map[i + 6][j + 6]->GetSite() == "政府") {
					int temjsonnum = 9107;
					for (int p = 0; p < 13; p++) {
						for (int q = 0; q < 13; q++) {
							Map[i + p][j + q]->SetSprite(LandMod[9][temjsonnum % 1000]);
							Map[i + p][j + q]->SetResource();
							Map[i + p][j + q]->Getjsonimagenum().push_back(temjsonnum);
							temjsonnum++;
						}
					}
				}
				else if (i + 9 < Map.size() && j + 9 < Map[0].size() && Map[i + 2][j + 2]->GetSite() == "市井" && Map[i + 4][j + 4]->GetSite() == "政府") {
					int temjsonnum = 9026;
					for (int p = 0; p < 9; p++) {
						for (int q = 0; q < 9; q++) {
							Map[i + p][j + q]->SetSprite(LandMod[9][temjsonnum % 1000]);
							Map[i + p][j + q]->SetResource();
							Map[i + p][j + q]->Getjsonimagenum().push_back(temjsonnum);
							temjsonnum++;
						}
					}

				}
				else if(i + 5 < Map.size() && j + 5 < Map[0].size()){
					int temjsonnum = 9001;
					for (int p = 0; p < 5; p++) {
						for (int q = 0; q < 5; q++) {
							Map[i + p][j + q]->SetSprite(LandMod[9][temjsonnum % 1000]);
							Map[i + p][j + q]->SetResource();
							Map[i + p][j + q]->Getjsonimagenum().push_back(temjsonnum);
							temjsonnum++;
						}
					}
				}
			}
			else if (Map[i][j]->GetSite() == "云朵") {
				Map[i][j]->SetSprite(LandMod[0][4]);
				Map[i][j]->Getjsonimagenum().push_back(4);
			}
		}
	}
}

void WareHouse::LOAD() {
	InitDataBase();
	InitFont();
	InitTexture();
	InitSprite();
	ADDLANDMOD();
	std::ifstream file(L"savegame.json");
	if (file.is_open()) {
		std::vector<char> ioBuf(1024 * 1024);
		file.rdbuf()->pubsetbuf(ioBuf.data(), ioBuf.size());

		nlohmann::json j;
		file >> j;
		file.close();

		// 加载地图边界
		HMIN = j["HMIN"];
		WMIN = j["WMIN"];
		HMAX = j["HMAX"];
		WMAX = j["WMAX"];
		CityGroupSize = j["CityGroupSize"];
		CountyGroupSize = j["CountyGroupSize"];
		ProvinceGroupSize = j["ProvinceGroupSize"];
		BuildGroupSize = j["BuildGroupSize"];
		ColorGroupSize = j["ColorGroupSize"];
		InitIsColorGroup();
		PlayerGroupSize = j["PlayerGroupSize"];

		// 清空所有数据
		CityGroup.clear();
		CityGroup.resize(CityGroupSize);
		CountyGroup.clear();
		CountyGroup.resize(CountyGroupSize);
		ProvinceGroup.clear();
		ProvinceGroup.resize(ProvinceGroupSize);
		BuildGroup.clear();
		BuildGroup.resize(BuildGroupSize);
		PlayerGroup.clear();
		PlayerGroup.resize(PlayerGroupSize);
		ColorGroup.clear();
		ColorGroup.resize(ColorGroupSize);
		Map.clear();
		Map.resize(HMAX - HMIN + 1);
		for (int i = 0; i < Map.size(); i++) {
			Map[i].resize(WMAX - WMIN + 1);
		}

		int ans = 0;

		//加载颜色库
		ans = 0;
		for (auto& colorJson : j["ColorGroup"]) {
			sf::Color color;
			fromcolorjson(colorJson, color);
			ColorGroup[ans] = color;
			ans++;
		}

		// 加载玩家
		ans = 0;
		for (auto& playerJson : j["PlayerGroup"]) {
			auto player = std::make_shared<Player>();
			player->fromjson(playerJson);
			if (player->Getjsoncolor() != -1) {
				player->SetColor(ColorGroup[player->Getjsoncolor()], player->Getjsoncolor());
				IsColorGroup[player->Getjsoncolor()] = 1;
			}
			PlayerGroup[ans] = player;
			ans++;
		}

		// 加载省
		ans = 0;
		for (auto& provinceJson : j["ProvinceGroup"]) {
			auto province = std::make_shared<Province>();
			province->fromjson(provinceJson);
			if (province->Getjsonplayerid() != -1) {
				province->JoinPlayer(PlayerGroup[province->Getjsonplayerid()]);
			}
			ProvinceGroup[ans] = province;
			ans++;
		}

		// 加载郡
		ans = 0;
		for (auto& countyJson : j["CountyGroup"]) {
			auto county = std::make_shared<County>();
			county->fromjson(countyJson);
			if (county->Getjsonplayerid() != -1) {
				county->JoinPlayer(PlayerGroup[county->Getjsonplayerid()]);
			}
			if (county->Getjsonprovinceid() != -1) {
				county->JsonJoinProvince(ProvinceGroup[county->Getjsonprovinceid()]);
			}
			CountyGroup[ans] = county;
			ans++;
		}

		// 加载县
		ans = 0;
		for (auto& cityJson : j["CityGroup"]) {
			auto city = std::make_shared<City>();
			city->fromjson(cityJson);
			if (city->Getjsonplayerid() != -1) {
				city->JoinPlayer(PlayerGroup[city->Getjsonplayerid()]);
			}
			if (city->Getjsonprovinceid() != -1) {
				city->JsonJoinProvince(ProvinceGroup[city->Getjsonprovinceid()]);
			}
			if (city->Getjsoncountyid() != -1) {
				city->JsonJoinCounty(CountyGroup[city->Getjsoncountyid()]);
			}
			CityGroup[ans] = city;
			ans++;
		}

		// 加载地图格子
		for (auto& rowJson : j["Map"]) {
			std::vector<std::shared_ptr<Grid>> row;
			for (auto& gridJson : rowJson) {
				auto grid = std::make_shared<Grid>();
				grid->fromjson(gridJson);
				grid->GetGrid().setPosition({ static_cast<float>(grid->GetX()),static_cast<float>(grid->GetY()) });
				if (grid->Getjsonplayerid() != -1) {
					grid->JsonJoinPlayer(PlayerGroup[grid->Getjsonplayerid()]);
				}
				if (grid->Getjsoncityid() != -1) {
					grid->JsonJoinCity(CityGroup[grid->Getjsoncityid()]);
					CityGroup[grid->Getjsoncityid()]->GetCityMaps().push_back(*grid);
				}
				if (grid->Getjsonimagenum().size() != 0) {
					for (int& temnum : grid->Getjsonimagenum()) {
						if (temnum >= 90) {
							grid->SetSprite1(LandMod[temnum / 1000][temnum % 1000]);
						}
						else {
							grid->SetSprite1(LandMod[temnum / 10][temnum % 10]);
						}
					}
				}
				Map[grid->GetY() - HMIN][grid->GetX() - WMIN] = grid;
			}
		}

		// 加载建筑
		ans = 0;
		for (auto& buildJson : j["BuildGroup"]) {
			Build build;
			build.fromjson(buildJson);
			BuildGroup[ans] = build;
			ans++;
		}

		//加载剩余物件
		for (auto& player : PlayerGroup) {
			if (player->Getjsoncapital() != -1) {
				player->SetCapital(CityGroup[player->Getjsoncapital()]);
			}
			if (player->IsLive()) {
				player->SetBelongCity(CityGroup[player->Getjsonbelongcity()]);
			}
		}
	}
	else {
		CityGroup.reserve(120);
		CountyGroup.reserve(10);
		ProvinceGroup.reserve(1);
		//加入一大堆东西
		ADDCITY();
		ADDCOUNTY();
		ADDPROVINCE();
		ADDMAP();
		ADDBUILD();
		ADDCOLOR();
	}
	InitChunk();
}

void WareHouse::SAVE() {
	PlayerGroupSize = PlayerGroup.size();
	nlohmann::json j;
	
	// 保存地图边界
	j["HMIN"] = HMIN;
	j["WMIN"] = WMIN;
	j["HMAX"] = HMAX;
	j["WMAX"] = WMAX;
	j["CityGroupSize"] = CityGroupSize;
	j["CountyGroupSize"] = CountyGroupSize;
	j["ProvinceGroupSize"] = ProvinceGroupSize;
	j["BuildGroupSize"] = BuildGroupSize;
	j["ColorGroupSize"] = ColorGroupSize;
	j["PlayerGroupSize"] = PlayerGroupSize;
	
	j["Map"] = Map;
	j["CityGroup"] = CityGroup;
	j["CountyGroup"] = CountyGroup;
	j["ProvinceGroup"] = ProvinceGroup;
	j["PlayerGroup"] = PlayerGroup;
	j["BuildGroup"] = BuildGroup;
	
	//保存颜色库
	nlohmann::json colorJson;
	for (auto& color : ColorGroup) {
		colorJson.push_back(tocolorjson(color));
	}
	j["ColorGroup"] = colorJson;
	
	// 写入文件
	std::ofstream file(L"savegame.json");
	if (file.is_open()) {
		file << j.dump(-1);
		file.close();
	}
}

WareHouse::WareHouse() :
	DataBase_(DataWarehouse::getInstance())
{
	LOAD();
}

WareHouse::~WareHouse() {
	SAVE();
}

WareHouse& WareHouse::getInstance(){
	return instance;
}

void WareHouse::SetHMIN(int x){
	HMIN = x;
}

void WareHouse::SetWMIN(int x){
	WMIN = x;
}

void WareHouse::SetHMAX(int x){
	HMAX = x;
}

void WareHouse::SetWMAX(int x){
	WMAX = x;
}

void WareHouse::SetCityGroupSize(int x){
	CityGroupSize = x;
}

void WareHouse::SetCountyGroupSize(int x){
	CountyGroupSize = x;
}

void WareHouse::SetProvinceGroupSize(int x){
	ProvinceGroupSize = x;
}

void WareHouse::SetBuildGroupSize(int x){
	BuildGroupSize = x;
}

void WareHouse::SetColorGroupSize(int x){
	ColorGroupSize = x;
}

void WareHouse::SetPlayerGroupSize(int x){
	PlayerGroupSize = x;
}

void WareHouse::importData() {
	if (this->hasImported) {
		std::cout << "数据已导入，禁止重复调用！" << std::endl;
		return;
	}

	this->hasImported = true;
}

int WareHouse::GetHMIN()const {
	return HMIN;
}
						
int WareHouse::GetWMIN()const {
	return WMIN;
}
						
int WareHouse::GetHMAX()const {
	return HMAX;
}
						
int WareHouse::GetWMAX()const {
	return WMAX;
}

int WareHouse::GetCityGroupSize() const{
	return CityGroupSize;
}

int WareHouse::GetCountyGroupSize() const{
	return CountyGroupSize;
}

int WareHouse::GetProvinceGroupSize() const{
	return ProvinceGroupSize;
}

int WareHouse::GetBuildGroupSize() const{
	return BuildGroupSize;
}

int WareHouse::GetColorGroupSize() const{
	return ColorGroupSize;
}

int WareHouse::GetPlayerGroupSize() const{
	return PlayerGroupSize;
}

bool WareHouse::GetIsExistDataBase()const {
	return IsExistDataBase;
}

std::vector<std::vector<sf::Texture>>& WareHouse::GetLandMod() {
	return LandMod;
}

std::vector<std::shared_ptr<City>>& WareHouse::GetCityGroup() {
	return CityGroup;
}

std::vector<std::shared_ptr<County>>& WareHouse::GetCountyGroup() {
	return CountyGroup;
}

std::vector<std::shared_ptr<Province>>& WareHouse::GetProvinceGroup() {
	return ProvinceGroup;
}

std::vector<std::vector<std::shared_ptr<Grid>>>& WareHouse::GetMap() {
	return Map;
}

std::vector<Build>& WareHouse::GetBuildGroup() {
	return BuildGroup;
}

std::vector<sf::Font>& WareHouse::GetFontGroup(){
	return FontGroup;
}

std::vector<sf::Color>& WareHouse::GetColorGroup() {
	return ColorGroup;
}

std::vector<bool>& WareHouse::GetIsColorGroup(){
	return IsColorGroup;
}

std::vector<std::shared_ptr<Player>>& WareHouse::GetPlayerGroup() {
	return PlayerGroup;
}

std::vector<sf::Sprite>& WareHouse::GetSpriteGroup(){
	return SpriteGroup;
}

std::vector<Chunk>& WareHouse::GetChunkGroup(){
	return MyChunk;
}

std::vector<Chunk>& WareHouse::GetCChunkGroup(){
	return MyCChunk;
}
