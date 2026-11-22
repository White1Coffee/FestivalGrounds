#pragma once
#include "core/Types.hpp"
#include <array>
#include <vector>
namespace fg {
class World {
public:
    World();
    bool inBounds(TilePos p) const;
    Tile& at(TilePos p); const Tile& at(TilePos p) const;
    bool place(BuildingType type, TilePos p, int rotation=0);
    void remove(TilePos p);
    bool hasPathAccess(TilePos p) const;
    std::vector<TilePos> buildings(BuildingType type) const;
    std::vector<TilePos> pathTiles() const;
    TilePos entrance() const { return entrance_; }
private:
    std::array<Tile, kMapWidth*kMapHeight> tiles_{};
    TilePos entrance_{kMapWidth/2, kMapHeight/2};
    std::size_t index(TilePos p) const { return static_cast<std::size_t>(p.y*kMapWidth+p.x); }
};
}
