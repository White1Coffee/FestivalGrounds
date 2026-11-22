#pragma once
#include "core/Types.hpp"
#include "world/World.hpp"
#include <SDL.h>
namespace fg { struct InputState { Tool tool{Tool::Path}; TilePos hover{-1,-1}; int cameraX{},cameraY{}; bool quit{}; bool nextDay{}; bool rotate{}; bool togglePause{}; }; class Input { public: static void update(InputState&,const World&,int width,int height,float dt); }; }
