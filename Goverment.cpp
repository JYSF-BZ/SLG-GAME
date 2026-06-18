#include "Goverment.h"
#include"Player.h"

Goverment::Goverment(){
	ID = 0;
	id = 0;
	Site = "";
	isBelong = false;
}

int Goverment::GetID()const{
	return ID;
}

int Goverment::Getid()const{
	return id;
}

int Goverment::Getjsonplayerid()const {
	return jsonplayerid;
}

const std::string& Goverment::GetName()const{
	return Name;
}

const std::string& Goverment::GetSite()const{
	return Site;
}

std::shared_ptr<Player> Goverment::GetBelongPlayer(){
	return BelongPlayer.lock();
}

std::unordered_set<int>& Goverment::GetChunkID(){
	return ChunkID;
}

void Goverment::SetBelongPlayer(const std::shared_ptr<Player>& player){
	BelongPlayer = player;
	jsonplayerid = player->GetID();
}

void Goverment::SetFourBorderID(int num){
	int wmin = 0xFFFF, wmax = 0, hmin = 0xFFFF, hmax = 0;
	for (auto& id : ChunkID) {
		if (id % num < wmin) {
			ID_Size.Wmin = id;
			wmin = id % num;
		}
		if (id % num > wmax) {
			ID_Size.Wmax = id;
			wmax = id % num;
		}
		if (id / num < hmin) {
			ID_Size.Hmin = id;
			hmin = id / num;
		}
		if (id / num > hmax) {
			ID_Size.Hmax = id;
			hmax = id / num;
		}
	}
}

void Goverment::Setid(int x){
	id = x;
}

bool Goverment::IsBelong()const{
	return isBelong;
}

Goverment::size Goverment::GetOnlyMapSize()const{
	return OnlyMapSize;
}

Goverment::size Goverment::GetID_Size()const{
	return ID_Size;
}
