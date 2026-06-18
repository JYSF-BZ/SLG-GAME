#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>
#include<nlohmann/json.hpp>
#include<vector>
#include"Grid.h"
#include"City.h"

class Chunk {
private:
    static const int SIZE = 64;

    std::vector<std::vector<std::shared_ptr<Grid>>> grids{ SIZE, std::vector<std::shared_ptr<Grid>>(SIZE) };
    sf::RenderTexture ChunkTexture;
    sf::RenderTexture ChunkOutlineTexture;
    sf::Sprite ChunkSprite;
    sf::Sprite ChunkOutlineSprite;
    bool isdirty = true;
    bool IsOutline = false;

private:
    void SetOutlineColor(sf::RenderTexture& tartexture,const std::shared_ptr<Grid>& targrid, sf::Color temcolor, int i, int j, bool IsCity = 1)const;

public:
    Chunk();
    void ReDraw(int ID, bool Ctype);
    void SetDirty();
    void SetIsOutline(bool IsOpen);
    void JoinGrid(int row, int col, const std::shared_ptr<Grid>& targrid);
    void Draw(sf::RenderTarget& target, int ID = -1, bool Ctype = 0);
    void SetPosition(float x, float y);
    void SetScale(float scale);
    int GetSize()const;
    float GetSpriteTextureSize()const;
    const sf::Sprite& GetSprite()const;
};