#pragma once

class City;
class County;
class Province;
class Player;

#include<iostream>
#include<unordered_set>
#include<SFML/GpuPreference.hpp>
#include<nlohmann/json.hpp>
#include<vector>

class Goverment {
public:
	struct size {
		int Hmax = 0, Hmin = 0xFFFF, Wmax = 0, Wmin = 0xFFFF;
	};

protected:
	int jsonplayerid = -1;

	size OnlyMapSize;
	size ID_Size;
	int ID, id;
	std::string Name;
	std::string Site;
	std::weak_ptr<Player> BelongPlayer;
	std::unordered_set<int> ChunkID;
	bool isBelong;

public:
	Goverment();

	int GetID()const;
	int Getid()const;
	int Getjsonplayerid()const;
	const std::string& GetName()const;
	const std::string& GetSite()const;
	std::shared_ptr<Player> GetBelongPlayer();
	std::unordered_set<int>& GetChunkID();
	void SetBelongPlayer(const std::shared_ptr<Player>& player);
	void SetFourBorderID(int num);
	void Setid(int x);
	bool IsBelong()const;
	size GetOnlyMapSize()const;
	size GetID_Size()const;

	virtual void JoinPlayer(const std::shared_ptr<Player>& player) = 0;
	virtual void fromjson(const nlohmann::json& j) = 0;
	virtual nlohmann::json tojson()const = 0;

	virtual ~Goverment() = default;
};