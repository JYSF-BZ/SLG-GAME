#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<nlohmann/json.hpp>
#include<fstream>
#include<sstream>

class Build {
private:
	int id;
	std::string Name;
	std::vector<int>LoseResourch;

public:
	int Getid();
	Build();
	Build(const std::string& tarname, const int& losegrain, const int& loseiron, const int& losetimber, const int& losestone, const int& losemoney, const int& buildid);
	const std::vector<int>& GetLoseResourch() const;
	const std::string& GetName()const;
	void Setid(int x);
	void fromjson(const nlohmann::json& j);
	nlohmann::json tojson()const;
};
