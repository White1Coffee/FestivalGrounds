#include "simulation/Simulation.hpp"
#include "pathfinding/Pathfinder.hpp"
#include <algorithm>
#include <cmath>
#include <random>
namespace fg {
namespace { std::mt19937 rng{std::random_device{}()}; float random(float a,float b){return std::uniform_real_distribution<float>(a,b)(rng);} }
Simulation::Simulation(World&w,Economy&e):world_(w),economy_(e),pathfinder_(w){}
bool Simulation::routeTo(Visitor&v,TilePos goal){auto r=pathfinder_.find(v.tile,goal);if(r.empty())return false;v.route=std::move(r);v.routeIndex=v.route.size()>1?1:0;v.target=goal;v.routeRevision=world_.navigationRevision();v.moveProgress=0.0f;return true;}
void Simulation::spawnVisitor(){auto paths=world_.pathTiles();if(paths.empty())return;Visitor v;v.id=nextId_++;v.tile=world_.entrancePath();v.position={static_cast<float>(v.tile.x),static_cast<float>(v.tile.y)};v.stayTimer=random(180,260);v.speed=random(1.7f,2.5f);v.decisionTimer=random(0.1f,1.2f);visitors_.push_back(v);report_.admitted++;}
void Simulation::decide(Visitor&v){
    auto food=world_.buildings(BuildingType::FoodStall),toilets=world_.buildings(BuildingType::Toilet),stages=world_.buildings(BuildingType::Stage);
    auto nearest=[&](const std::vector<TilePos>&items)->TilePos{TilePos best{};float bd=INFINITY;constexpr TilePos d[]={{1,0},{-1,0},{0,1},{0,-1}};for(auto facility:items)for(auto dir:d){TilePos p{facility.x+dir.x,facility.y+dir.y};if(!world_.inBounds(p)||!world_.at(p).path)continue;float score=static_cast<float>(std::abs(p.x-v.tile.x)+std::abs(p.y-v.tile.y));if(score<bd){bd=score;best=p;}}return best;};
    if(v.state==VisitorState::Leaving){if(!routeTo(v,world_.entrancePath())){v.route.clear();v.routeIndex=0;}return;}
    if(v.needs.hunger>.64f&&!food.empty()&&routeTo(v,nearest(food))){v.state=VisitorState::GoingToFood;return;}
    if(v.needs.thirst>.9f){v.needs.happiness-=.02f;}
    if(v.needs.patience<.08f||v.needs.energy<.16f||v.stayTimer<=0){v.state=VisitorState::Leaving;if(!routeTo(v,world_.entrancePath())){v.route.clear();v.routeIndex=0;}return;}
    if(v.needs.hunger>.45f&&!food.empty()&&routeTo(v,nearest(food))){v.state=VisitorState::GoingToFood;return;}
    if(v.needs.patience<.55f&&!toilets.empty()&&routeTo(v,nearest(toilets))){v.state=VisitorState::GoingToToilet;return;}
    if(!stages.empty()&&random(0,1)<.56f&&routeTo(v,nearest(stages))){v.state=VisitorState::GoingToStage;return;}
    auto paths=world_.pathTiles();if(!paths.empty()){for(int tries=0;tries<8;++tries)if(routeTo(v,paths[static_cast<std::size_t>(random(0,static_cast<float>(paths.size())))%paths.size()]))break;}v.state=VisitorState::Wandering;
}
void Simulation::depart(std::size_t i){auto&v=visitors_[i];float sat=std::clamp((v.needs.happiness*.65f+v.needs.energy*.2f+(1-v.needs.hunger)*.15f)*100.0f,0.0f,100.0f);report_.satisfactionTotal+=sat;report_.satisfactionCount++;report_.departed++;visitors_.erase(visitors_.begin()+static_cast<std::ptrdiff_t>(i));}
void Simulation::update(float dt){if(paused)return;dayTime_+=dt;spawnTimer_+=dt;
    // One in-game day lasts about six real-time minutes; arrival rate rises through afternoon.
    const float arrival=dayTime_<100?1.4f:dayTime_<280?2.5f:0.8f;
    if(dayTime_<390&&spawnTimer_>=arrival){spawnTimer_=0;if(visitors_.size()<450)spawnVisitor();}
    for(std::size_t i=0;i<visitors_.size();){auto&v=visitors_[i];v.needs.hunger=std::min(1.0f,v.needs.hunger+dt*.0035f);v.needs.thirst=std::min(1.0f,v.needs.thirst+dt*.0022f);v.needs.energy=std::max(0.0f,v.needs.energy-dt*.0010f);v.needs.patience=std::max(0.0f,v.needs.patience-dt*.0005f);v.stayTimer-=dt;
        if(v.routeRevision!=world_.navigationRevision()){
            const bool mustRoute=!v.route.empty()||v.state==VisitorState::Leaving||!world_.at(v.tile).path;
            v.routeRevision=world_.navigationRevision();
            if(mustRoute){v.position={static_cast<float>(v.tile.x),static_cast<float>(v.tile.y)};TilePos newGoal=v.state==VisitorState::Leaving?world_.entrancePath():v.target;
                if(!routeTo(v,newGoal)){v.route.clear();v.routeIndex=0;if(v.state==VisitorState::Leaving){depart(i);continue;}v.state=VisitorState::Wandering;v.decisionTimer=0.0f;}
            }
        }
        if(v.state==VisitorState::Leaving&&v.tile==world_.entrancePath()&&v.routeIndex>=v.route.size()){depart(i);continue;}
        if(v.state==VisitorState::GoingToFood&&v.tile==v.target&&v.routeIndex>=v.route.size()){if(!world_.buildings(BuildingType::FoodStall).empty()&&v.needs.hunger>.35f){economy_.sellFood(report_);v.needs.hunger=std::max(0.0f,v.needs.hunger-.72f);v.needs.thirst=std::min(1.0f,v.needs.thirst+.14f);v.needs.happiness=std::min(1.0f,v.needs.happiness+.08f);}v.state=VisitorState::Wandering;v.decisionTimer=0.3f;}
        if(v.state==VisitorState::GoingToToilet&&v.tile==v.target&&v.routeIndex>=v.route.size()){v.needs.patience=std::min(1.0f,v.needs.patience+.48f);v.needs.happiness=std::min(1.0f,v.needs.happiness+.04f);v.state=VisitorState::Wandering;v.decisionTimer=.4f;}
        if(v.state==VisitorState::GoingToStage&&v.tile==v.target&&v.routeIndex>=v.route.size()){v.needs.happiness=std::min(1.0f,v.needs.happiness+dt*.025f);v.needs.energy=std::max(0.0f,v.needs.energy-dt*.0007f);}
        v.decisionTimer-=dt;if(v.decisionTimer<=0&&(v.routeIndex>=v.route.size()||v.state==VisitorState::Wandering)){decide(v);v.decisionTimer=random(2.5f,6.0f);if(v.state==VisitorState::Leaving&&v.route.empty()){depart(i);continue;}}
        if(v.routeIndex<v.route.size()){v.moveProgress=std::fmod(v.moveProgress+dt,1.0f);TilePos next=v.route[v.routeIndex];Vec2 target{static_cast<float>(next.x),static_cast<float>(next.y)};float dx=target.x-v.position.x,dy=target.y-v.position.y,len=std::sqrt(dx*dx+dy*dy);float step=v.speed*dt;if(len<=step||len<.01f){v.position=target;v.tile=next;v.routeIndex++;}else{v.position.x+=dx/len*step;v.position.y+=dy/len*step;}}
        ++i;
    }
    if(dayTime_>=480)nextDay();
}
void Simulation::nextDay(){dayTime_=0;spawnTimer_=0;lastReport_=report_;economy_.closeDay(report_);}
}
