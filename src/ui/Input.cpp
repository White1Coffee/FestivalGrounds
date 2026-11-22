#include "ui/Input.hpp"
#include "rendering/Renderer.hpp"
namespace fg {
void Input::update(InputState&s,const World&,int width,int height,float dt){SDL_Event ev;while(SDL_PollEvent(&ev)){if(ev.type==SDL_QUIT)s.quit=true;if(ev.type==SDL_KEYDOWN&&!ev.key.repeat){switch(ev.key.keysym.sym){case SDLK_ESCAPE:s.quit=true;break;case SDLK_1:s.tool=Tool::Path;break;case SDLK_2:s.tool=Tool::FoodStall;break;case SDLK_3:s.tool=Tool::Toilet;break;case SDLK_4:s.tool=Tool::Stage;break;case SDLK_5:s.tool=Tool::Entrance;break;case SDLK_r:s.rotate=true;break;case SDLK_SPACE:s.togglePause=true;break;case SDLK_n:s.nextDay=true;break;case SDLK_HOME:s.cameraX=s.cameraY=0;break;default:break;}}
 if(ev.type==SDL_MOUSEBUTTONDOWN){int x,y;SDL_GetMouseState(&x,&y);Renderer projection(nullptr,width,height);s.hover=projection.screenToTile(x,y,s.cameraX,s.cameraY);}}
 const Uint8*k=SDL_GetKeyboardState(nullptr);int speed=static_cast<int>(400*dt);if(k[SDL_SCANCODE_LEFT]||k[SDL_SCANCODE_A])s.cameraX-=speed;if(k[SDL_SCANCODE_RIGHT]||k[SDL_SCANCODE_D])s.cameraX+=speed;if(k[SDL_SCANCODE_UP]||k[SDL_SCANCODE_W])s.cameraY-=speed;if(k[SDL_SCANCODE_DOWN]||k[SDL_SCANCODE_S])s.cameraY+=speed;
 int mx,my;SDL_GetMouseState(&mx,&my);Renderer projection(nullptr,width,height);s.hover=projection.screenToTile(mx,my,s.cameraX,s.cameraY);
}
}
