#pragma once
#include "world/World.hpp"
namespace fg { class Pathfinder { public: static std::vector<TilePos> find(const World& world, TilePos start, TilePos goal); }; }
