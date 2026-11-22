#pragma once
#include "core/Types.hpp"
#include <array>
#include <cstdint>
#include <vector>
namespace fg {
class World {
public:
    explicit World(std::uint32_t seed=0xF3571A1u);
    void generate(std::uint32_t seed);
    bool inBounds(TilePos p) const;
    bool canPlace(BuildingType type,TilePos p) const;
    Tile& at(TilePos p); const Tile& at(TilePos p) const;
    bool place(BuildingType type, TilePos p, int rotation=0);
    void remove(TilePos p);
    bool hasPathAccess(TilePos p) const;
    std::vector<TilePos> buildings(BuildingType type) const;
    std::vector<TilePos> pathTiles() const;
    TilePos entrance() const { return entrance_; }
    TilePos entrancePath() const;
private:
    std::array<Tile, kMapWidth*kMapHeight> tiles_{};
    TilePos entrance_{kMapWidth/2, kMapHeight/2};
    std::size_t index(TilePos p) const { return static_cast<std::size_t>(p.y*kMapWidth+p.x); }
};
}
