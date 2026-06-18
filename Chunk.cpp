#include "Chunk.h"

void Chunk::SetOutlineColor(sf::RenderTexture& tartexture,const std::shared_ptr<Grid>& targrid, sf::Color temcolor, int i, int j, bool IsCity) const{
    sf::RectangleShape line({ 1.0f, 1.0f });
    line.setFillColor(temcolor);
    if (targrid->IsUp(IsCity) || targrid->IsDown(IsCity) || targrid->IsLeft(IsCity) || targrid->IsRight(IsCity)) {
        line.setPosition({ (float)j,(float)i });
    }

    if (targrid->IsUp(IsCity) || targrid->IsDown(IsCity) || targrid->IsLeft(IsCity) || targrid->IsRight(IsCity)) {
        tartexture.draw(line);
    }
}

Chunk::Chunk() :
    ChunkTexture(),
    ChunkSprite(ChunkTexture.getTexture()),
    ChunkOutlineSprite(ChunkOutlineTexture.getTexture())
{
    ChunkTexture.resize({ (unsigned int)SIZE, (unsigned int)SIZE });
    ChunkTexture.setSmooth(false);
    ChunkSprite.setTexture(ChunkTexture.getTexture(), true);

    ChunkOutlineTexture.resize({ (unsigned int)SIZE, (unsigned int)SIZE });
    ChunkOutlineTexture.setSmooth(false);
    ChunkOutlineSprite.setTexture(ChunkOutlineTexture.getTexture(), true);
}

void Chunk::ReDraw(int ID, bool Ctype) {
    if (!isdirty) {
        return;
    }

    // 清空画布为全透明
    ChunkTexture.clear(sf::Color::Transparent);
    ChunkOutlineTexture.clear(sf::Color::Transparent);

    //判断行政等级
    int temnum = 1;
    bool IsAll = 0;
    if (ID == 1000000) {
        temnum = 10000000;
        IsAll = 1;
    }
    else if (ID % 100 == 0) {
        if (ID % 10000 == 0) {
            if (ID % 1000000 == 0) {
                std::cout << ID << "：ID错误" << std::endl;
                return;
            }
            else {
                temnum = 10000;
            }
        }
        else {
            temnum = 100;
        }
    }

    // 遍历64*64所有格子，非空就绘制
    for (int i = 0;i < grids.size();i++) {
        for (int j = 0;j < grids[i].size();j++) {
            if (grids[i][j] != nullptr && grids[i][j]->IsMap()) {
                if (!Ctype && IsAll) {
                    grids[i][j]->GetSprite().setPosition({ static_cast<float>(j),static_cast<float>(i) });
                    grids[i][j]->GetSprite().setScale({ 1.0f / grids[i][j]->GetSprite().getTexture().getSize().x,1.0f / grids[i][j]->GetSprite().getTexture().getSize().y });
                    ChunkTexture.draw(*grids[i][j]);
                }
                else if (Ctype) {
                    if (grids[i][j]->GetBelongCity()->GetID() / temnum == ID / temnum) {
                        sf::RectangleShape mask;
                        mask.setPosition({ static_cast<float>(j), static_cast<float>(i) });
                        mask.setSize({ 1.f, 1.f });
                        sf::Color temcolor = sf::Color::White;
                        if (grids[i][j]->GetBelongCity()->GetBelongPlayer() != nullptr) {
                            temcolor = grids[i][j]->GetBelongCity()->GetBelongPlayer()->GetColor();
                        }
                        mask.setFillColor(temcolor);
                        ChunkTexture.draw(mask);
                        SetOutlineColor(ChunkTexture, grids[i][j], sf::Color::Black, i, j);
                    }
                }
                else if (grids[i][j]->GetBelongCity()->GetID() / 10000 == ID / 10000) {
                    grids[i][j]->GetSprite().setPosition({ (float)j,(float)i });
                    grids[i][j]->GetSprite().setScale({ 1.0f / grids[i][j]->GetSprite().getTexture().getSize().x,1.0f / grids[i][j]->GetSprite().getTexture().getSize().y });
                    
                    sf::RenderStates states;
                    states.blendMode = sf::BlendNone;
                    ChunkTexture.draw(*grids[i][j],states);
                    if (temnum <= 100 && grids[i][j]->GetBelongCity()->GetID() / 100 == ID / 100) {
                        SetOutlineColor(ChunkOutlineTexture, grids[i][j], sf::Color::White, i, j, 0);
                        ChunkOutlineSprite.setPosition(ChunkSprite.getPosition());
                        ChunkOutlineSprite.setScale(ChunkSprite.getScale());
                    }
                    if (temnum == 1 && grids[i][j]->GetBelongCity()->GetID() == ID) {
                        SetOutlineColor(ChunkOutlineTexture, grids[i][j], sf::Color::Red, i, j);
                        ChunkOutlineSprite.setPosition(ChunkSprite.getPosition());
                        ChunkOutlineSprite.setScale(ChunkSprite.getScale());
                    }
                    
                }
            }
        }
    }

    // 提交绘制数据到内部纹理并更新纹理
    ChunkTexture.display();
    ChunkSprite.setTexture(ChunkTexture.getTexture(), true);

    if (IsOutline) {
        ChunkOutlineTexture.display();
        ChunkOutlineSprite.setTexture(ChunkOutlineTexture.getTexture(), true);
    }
    isdirty = false;
}

void Chunk::SetDirty(){
    isdirty = true;
}

void Chunk::SetIsOutline(bool IsOpen){
    IsOutline = IsOpen;
}

void Chunk::JoinGrid(int row, int col, const std::shared_ptr<Grid>& targrid) {
    grids[row][col] = targrid;
    isdirty = true;
}

void Chunk::Draw(sf::RenderTarget& target, int ID,bool Ctype) {
    // 脏则先重绘
    if (isdirty) {
        ReDraw(ID, Ctype);
    }
    // 把整个区块一次性画到窗口
    
    target.draw(ChunkSprite);
    if (IsOutline) {
        target.draw(ChunkOutlineSprite);
    }
}

void Chunk::SetPosition(float x, float y){
    ChunkSprite.setPosition({ x,y });
}

void Chunk::SetScale(float scale){
    ChunkSprite.setScale({ scale,scale });
}

int Chunk::GetSize()const{
    return SIZE;
}

float Chunk::GetSpriteTextureSize()const{
    //正方形肯定边长就行
    return ChunkSprite.getTexture().getSize().x;
}

const sf::Sprite& Chunk::GetSprite()const{
    return ChunkSprite;
}
