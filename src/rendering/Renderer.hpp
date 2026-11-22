#pragma once
#include "core/Types.hpp"
#include "economy/Economy.hpp"
#include "simulation/Simulation.hpp"
#include "world/World.hpp"
#include <SDL.h>
namespace fg {
class Renderer {
public:
    Renderer(SDL_Renderer* renderer,int width,int height,float zoom=1.0f):renderer_(renderer),width_(width),height_(height),zoom_(zoom){}
    void draw(const World&,const Simulation&,const Economy&,Tool selected,TilePos hover,bool previewValid,int cameraX,int cameraY,float fps);
    TilePos screenToTile(int sx,int sy,int cameraX,int cameraY)const;
private:
    SDL_Renderer* renderer_;int width_,height_;float zoom_;
    SDL_Point iso(TilePos,int,int)const;
    void text(int x,int y,const char* value,SDL_Color color);
    void diamond(TilePos,int,int,SDL_Color);
    void building(TilePos,BuildingType,int,int);
};
}
