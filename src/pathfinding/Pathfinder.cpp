#include "pathfinding/Pathfinder.hpp"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <limits>
#include <queue>

namespace fg {
std::uint64_t Pathfinder::key(TilePos start,TilePos goal) {
    const auto startIndex=static_cast<std::uint32_t>(start.y*kMapWidth+start.x);
    const auto goalIndex=static_cast<std::uint32_t>(goal.y*kMapWidth+goal.x);
    return (static_cast<std::uint64_t>(startIndex)<<32)|goalIndex;
}

std::vector<TilePos> Pathfinder::find(TilePos start,TilePos goal) {
    if(!world_.inBounds(start)||!world_.inBounds(goal))return{};
    if(cachedRevision_!=world_.navigationRevision())clear();
    const auto routeKey=key(start,goal);
    if(const auto cached=routeCache_.find(routeKey);cached!=routeCache_.end())return cached->second;
    auto route=findUncached(start,goal);
    if(routeCache_.size()>=kMaxCachedRoutes)routeCache_.clear();
    routeCache_.emplace(routeKey,route);
    return route;
}

void Pathfinder::clear() {
    routeCache_.clear();
    cachedRevision_=world_.navigationRevision();
}

std::vector<TilePos> Pathfinder::findUncached(TilePos start,TilePos goal) const {
    // The goal must remain a walkable path. A visitor may start on a tile that
    // was just bulldozed; it can leave that tile, but every step after it must
    // land on an existing path tile.
    if(!world_.at(goal).path)return{};
    if(start==goal)return{start};

    struct Node { int f; int g; TilePos pos; };
    struct LowerPriority {
        bool operator()(const Node&a,const Node&b)const {
            if(a.f!=b.f)return a.f>b.f;
            return a.g<b.g;
        }
    };
    constexpr int count=kMapWidth*kMapHeight;
    constexpr TilePos directions[]={{1,0},{-1,0},{0,1},{0,-1}};
    const auto index=[](TilePos p){return p.y*kMapWidth+p.x;};
    const auto heuristic=[&](TilePos p){return std::abs(p.x-goal.x)+std::abs(p.y-goal.y);};
    std::array<int,count> cost;cost.fill(std::numeric_limits<int>::max());
    std::array<int,count> cameFrom;cameFrom.fill(-1);
    std::priority_queue<Node,std::vector<Node>,LowerPriority> open;
    cost[index(start)]=0;
    open.push({heuristic(start),0,start});

    while(!open.empty()) {
        const Node current=open.top();open.pop();
        if(current.g!=cost[index(current.pos)])continue;
        if(current.pos==goal)break;
        for(const auto direction:directions) {
            const TilePos next{current.pos.x+direction.x,current.pos.y+direction.y};
            if(!world_.inBounds(next)||!world_.at(next).path)continue;
            const int nextIndex=index(next);
            const int nextCost=current.g+1;
            if(nextCost>=cost[nextIndex])continue;
            cost[nextIndex]=nextCost;
            cameFrom[nextIndex]=index(current.pos);
            open.push({nextCost+heuristic(next),nextCost,next});
        }
    }

    if(cameFrom[index(goal)]<0)return{};
    std::vector<TilePos> route{goal};
    for(int cursor=index(goal);cursor!=index(start);) {
        cursor=cameFrom[cursor];
        if(cursor<0)return{};
        route.push_back({cursor%kMapWidth,cursor/kMapWidth});
    }
    std::reverse(route.begin(),route.end());
    return route;
}
}
