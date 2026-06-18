#include "Build.h"

int Build::Getid() {
	return id;
}

Build::Build() {
	std::cout << "Build的无参构造函数执行了" << std::endl;
	id = 0;
}

Build::Build(const std::string& tarname, const int& losegrain, const int& loseiron, const int& losetimber, const int& losestone, const int& losemoney, const int& buildid) {
	Name = tarname;
	LoseResourch.push_back(losegrain);
	LoseResourch.push_back(loseiron);
	LoseResourch.push_back(losetimber);
	LoseResourch.push_back(losestone);
	LoseResourch.push_back(losemoney);
	id = buildid;
}

const std::vector<int>& Build::GetLoseResourch() const {
	return LoseResourch;
}

const std::string& Build::GetName()const {
	return Name;
}

void Build::Setid(int x) {
	id = x;
}

void Build::fromjson(const nlohmann::json& j) {
	Name = j["Name"];
	id = j["id"];
	LoseResourch = j["LoseResourch"].get<std::vector<int>>();
}

nlohmann::json Build::tojson()const {
	nlohmann::json j;
	j["Name"] = Name;
	j["id"] = id;
	j["LoseResourch"] = LoseResourch;
	return j;
}
