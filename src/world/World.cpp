#include "world/World.hpp"
#include <random>
namespace fg {
World::World(std::uint32_t seed) { generate(seed); }
void World::generate(std::uint32_t seed) {
    entrance_={kMapWidth/2,kMapHeight/2};
    std::mt19937 random(seed);
    std::uniform_int_distribution<int> variation(0,15);
    for(auto& tile:tiles_) tile=Tile{Terrain::Grass,static_cast<std::uint8_t>(variation(random)),false,BuildingType::None,0};
    at(entrance_).building=BuildingType::Entrance;
    for(int x=entrance_.x-2;x<=entrance_.x+2;++x) at({x,entrance_.y+1}).path=true;
}
bool World::inBounds(TilePos p) const { return p.x>=0&&p.y>=0&&p.x<kMapWidth&&p.y<kMapHeight; }
Tile& World::at(TilePos p) { return tiles_[index(p)]; }
const Tile& World::at(TilePos p) const { return tiles_[index(p)]; }
bool World::hasPathAccess(TilePos p) const {
    constexpr TilePos d[]={{1,0},{-1,0},{0,1},{0,-1}};
    for(auto v:d){ TilePos n{p.x+v.x,p.y+v.y}; if(inBounds(n)&&at(n).path)return true; } return false;
}
TilePos World::entrancePath() const {
    constexpr TilePos dirs[]={{0,1},{1,0},{-1,0},{0,-1}};
    for(auto d:dirs){TilePos p{entrance_.x+d.x,entrance_.y+d.y};if(inBounds(p)&&at(p).path)return p;}
    return {entrance_.x,entrance_.y+1};
}
bool World::canPlace(BuildingType type,TilePos p) const {
    if(!inBounds(p)||type==BuildingType::None||at(p).path||at(p).building!=BuildingType::None)return false;
    if(type==BuildingType::Entrance)return hasPathAccess(p);
    if((type==BuildingType::FoodStall||type==BuildingType::Toilet||type==BuildingType::Stage)&&!hasPathAccess(p))return false;
    return true;
}
bool World::place(BuildingType type,TilePos p,int rotation) {
    if(!canPlace(type,p))return false;
    if(type==BuildingType::Path){at(p).path=true;return true;}
    if(type==BuildingType::Entrance)at(entrance_).building=BuildingType::None;
    at(p).building=type;at(p).rotation=((rotation%4)+4)%4;
    if(type==BuildingType::Entrance)entrance_=p;
    return true;
}
void World::remove(TilePos p) { if(!inBounds(p)||p==entrance_)return; at(p).building=BuildingType::None; at(p).path=false; }
std::vector<TilePos> World::buildings(BuildingType type) const { std::vector<TilePos> out; for(int y=0;y<kMapHeight;++y)for(int x=0;x<kMapWidth;++x)if(at({x,y}).building==type)out.push_back({x,y});return out; }
std::vector<TilePos> World::pathTiles() const { std::vector<TilePos> out; for(int y=0;y<kMapHeight;++y)for(int x=0;x<kMapWidth;++x)if(at({x,y}).path)out.push_back({x,y});return out; }
}
