#pragma once
#include "core/Types.hpp"
#include "core/EventBus.hpp"
#include "world/World.hpp"
#include <SDL.h>
namespace fg { struct InputState { Tool tool{Tool::Path}; TilePos hover{-1,-1}; int cameraX{},cameraY{602}; float zoom{1.0f}; }; class Input { public: static void update(InputState&,const World&,EventBus&,SDL_Renderer*,int width,int height,float dt); }; }
