#pragma once
#include "world/World.hpp"
#include <cstddef>
#include <cstdint>
#include <unordered_map>
namespace fg {
class Pathfinder {
public:
    explicit Pathfinder(const World& world):world_(world),cachedRevision_(world.navigationRevision()){}
    std::vector<TilePos> find(TilePos start,TilePos goal);
    void clear();
private:
    static std::uint64_t key(TilePos start,TilePos goal);
    std::vector<TilePos> findUncached(TilePos start,TilePos goal) const;
    const World& world_;
    std::uint64_t cachedRevision_{};
    std::unordered_map<std::uint64_t,std::vector<TilePos>> routeCache_;
    static constexpr std::size_t kMaxCachedRoutes=1024;
};
}
