#include "core/AssetManager.hpp"
#include "core/EventBus.hpp"
#include "core/Logger.hpp"
#include "economy/Economy.hpp"
#include "rendering/Renderer.hpp"
#include "simulation/Simulation.hpp"
#include "ui/Input.hpp"
#include "world/World.hpp"
#include <SDL.h>
#include <algorithm>
#include <chrono>
#include <cstdio>

int main(int,char**){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){fg::Logger::write(fg::LogLevel::Error,SDL_GetError());return 1;}
    SDL_Window*window=SDL_CreateWindow("Festival Grounds",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,800,SDL_WINDOW_RESIZABLE);
    if(!window){fg::Logger::write(fg::LogLevel::Error,SDL_GetError());SDL_Quit();return 1;}
    SDL_Renderer*renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
    if(!renderer){fg::Logger::write(fg::LogLevel::Error,SDL_GetError());SDL_DestroyWindow(window);SDL_Quit();return 1;}
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0");
    SDL_RenderSetLogicalSize(renderer,640,400);
    SDL_RenderSetIntegerScale(renderer,SDL_TRUE);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    fg::AssetManager assets(renderer);
    fg::World world;fg::Economy economy;fg::Simulation simulation(world,economy);fg::InputState input;fg::EventBus events;
    bool running=true;events.subscribe(fg::EventType::QuitRequested,[&](const fg::Event&){running=false;});events.subscribe(fg::EventType::PauseToggled,[&](const fg::Event&){simulation.paused=!simulation.paused;});events.subscribe(fg::EventType::NextDayRequested,[&](const fg::Event&){simulation.nextDay();});
    fg::Logger::write(fg::LogLevel::Info,"Festival Grounds started.");
    float accumulator=0;float fps=60.0f;auto previous=std::chrono::steady_clock::now();
    while(running){auto now=std::chrono::steady_clock::now();float frame=std::chrono::duration<float>(now-previous).count();previous=now;frame=std::min(frame,.25f);if(frame>0)fps=fps*.9f+(1.0f/frame)*.1f;constexpr int width=640,height=400;fg::Input::update(input,world,events,renderer,width,height,frame);
        int mx,my;Uint32 buttons=SDL_GetMouseState(&mx,&my);auto p=input.hover;fg::BuildingType type=fg::BuildingType::Path;switch(input.tool){case fg::Tool::Path:type=fg::BuildingType::Path;break;case fg::Tool::FoodStall:type=fg::BuildingType::FoodStall;break;case fg::Tool::Toilet:type=fg::BuildingType::Toilet;break;case fg::Tool::Stage:type=fg::BuildingType::Stage;break;case fg::Tool::Entrance:type=fg::BuildingType::Entrance;break;case fg::Tool::Bulldoze:break;}
        bool valid=world.inBounds(p);if(valid){auto&t=world.at(p);if(input.tool==fg::Tool::Path)valid=!t.path&&t.building==fg::BuildingType::None&&economy.canAfford(fg::definition(type).cost);else if(input.tool==fg::Tool::Bulldoze)valid=t.path||t.building!=fg::BuildingType::None;else valid=t.building==fg::BuildingType::None&&!t.path&&economy.canAfford(fg::definition(type).cost)&&((type==fg::BuildingType::Entrance)||world.hasPathAccess(p));}
        static Uint32 prevButtons=0;bool pressed=(buttons&SDL_BUTTON(SDL_BUTTON_LEFT))&&!(prevButtons&SDL_BUTTON(SDL_BUTTON_LEFT));bool right=(buttons&SDL_BUTTON(SDL_BUTTON_RIGHT))&&!(prevButtons&SDL_BUTTON(SDL_BUTTON_RIGHT));prevButtons=buttons;
        if(pressed&&valid){if(input.tool==fg::Tool::Bulldoze)world.remove(p);else if(type==fg::BuildingType::Path){if(economy.spend(5))world.at(p).path=true;}else if(type!=fg::BuildingType::Entrance&&economy.spend(fg::definition(type).cost))world.place(type,p);}
        if(right&&world.inBounds(p))world.remove(p);
        if(input.rotate){input.rotate=false;}
        accumulator+=frame;while(accumulator>=fg::kFixedStep){simulation.update(fg::kFixedStep);accumulator-=fg::kFixedStep;}
        fg::Renderer view(renderer,width,height,input.zoom);view.draw(world,simulation,economy,input.tool,p,valid,input.cameraX,input.cameraY,fps);SDL_RenderPresent(renderer);
    }
    fg::Logger::write(fg::LogLevel::Info,"Festival Grounds shutting down.");assets.clear();SDL_DestroyRenderer(renderer);SDL_DestroyWindow(window);SDL_Quit();return 0;
}
