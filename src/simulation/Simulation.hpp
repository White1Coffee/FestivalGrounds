#pragma once
#include "core/Types.hpp"
#include "economy/Economy.hpp"
#include "pathfinding/Pathfinder.hpp"
#include "world/World.hpp"
namespace fg {
class Simulation {
public:
    Simulation(World& world, Economy& economy);
    void update(float dt); void nextDay();
    const std::vector<Visitor>& visitors()const{return visitors_;}
    const DailyReport& report()const{return report_;}
    const DailyReport& lastReport()const{return lastReport_;}
    int day()const{return report_.day;} float dayTime()const{return dayTime_;}
    bool paused{};
private:
    void spawnVisitor(); void decide(Visitor& v); bool routeTo(Visitor& v,TilePos goal); void depart(std::size_t index);
    World& world_; Economy& economy_; Pathfinder pathfinder_; std::vector<Visitor> visitors_; DailyReport report_{}; DailyReport lastReport_{}; std::uint32_t nextId_{1}; float dayTime_{}; float spawnTimer_{};
};
}
