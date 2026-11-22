#include "pathfinding/Pathfinder.hpp"
#include <algorithm>
#include <cmath>
#include <queue>
namespace fg {
std::vector<TilePos> Pathfinder::find(const World& w,TilePos start,TilePos goal){
    if(!w.inBounds(start)||!w.inBounds(goal)||(!w.at(start).path&&start!=goal)||(!w.at(goal).path&&start!=goal))return{};
    struct Node{float f;TilePos p;}; struct Greater{bool operator()(const Node&a,const Node&b)const{return a.f>b.f;}};
    constexpr int n=kMapWidth*kMapHeight; constexpr TilePos d[]={{1,0},{-1,0},{0,1},{0,-1}};
    auto ix=[](TilePos p){return p.y*kMapWidth+p.x;}; auto h=[&](TilePos p){return std::abs(p.x-goal.x)+std::abs(p.y-goal.y);};
    std::array<float,n> cost; cost.fill(INFINITY); std::array<int,n> came; came.fill(-1); std::priority_queue<Node,std::vector<Node>,Greater> open;
    cost[ix(start)]=0;open.push({static_cast<float>(h(start)),start});
    while(!open.empty()){auto p=open.top().p;open.pop();if(p==goal)break;
        for(auto v:d){TilePos q{p.x+v.x,p.y+v.y};if(!w.inBounds(q)||(!w.at(q).path&&q!=goal))continue;const int qi=ix(q);float nc=cost[ix(p)]+1.0f;if(nc>=cost[qi])continue;cost[qi]=nc;came[qi]=ix(p);open.push({nc+static_cast<float>(h(q)),q});}}
    if(start!=goal&&came[ix(goal)]<0)return{}; std::vector<TilePos> route;int cur=ix(goal);route.push_back(goal);while(cur!=ix(start)){cur=came[cur];if(cur<0)return{};route.push_back({cur%kMapWidth,cur/kMapWidth});}std::reverse(route.begin(),route.end());return route;
}
}
