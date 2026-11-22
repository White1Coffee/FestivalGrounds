#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace fg {
constexpr int kMapWidth = 64;
constexpr int kMapHeight = 64;
constexpr int kTileW = 48;
constexpr int kTileH = 24;
constexpr float kFixedStep = 1.0f / 30.0f;

struct Vec2 { float x{}; float y{}; };
struct TilePos { int x{}; int y{}; friend bool operator==(TilePos, TilePos) = default; };
inline float distance(Vec2 a, Vec2 b) { const float x=a.x-b.x, y=a.y-b.y; return x*x+y*y; }

enum class Terrain : std::uint8_t { Grass, Mud };
enum class BuildingType : std::uint8_t { None, Path, FoodStall, Toilet, Stage, Entrance };
enum class Tool : std::uint8_t { Path, FoodStall, Toilet, Stage, Entrance, Bulldoze };
enum class VisitorState : std::uint8_t { Entering, Wandering, GoingToStage, GoingToFood, GoingToToilet, Leaving };
struct BuildingDef { BuildingType type; const char* name; int cost; };
inline constexpr std::array<BuildingDef, 6> kBuildingDefs{{
    {BuildingType::None,"Grass",0},{BuildingType::Path,"Path",5},
    {BuildingType::FoodStall,"Food stall",120},{BuildingType::Toilet,"Toilet",80},
    {BuildingType::Stage,"Small stage",450},{BuildingType::Entrance,"Entrance gate",0}
}};
inline constexpr BuildingDef definition(BuildingType t) { return kBuildingDefs[static_cast<std::size_t>(t)]; }
struct Tile { Terrain terrain{Terrain::Grass}; std::uint8_t variation{}; bool path{}; BuildingType building{BuildingType::None}; };
struct VisitorNeeds { float hunger{0.05f}; float thirst{0.05f}; float happiness{0.55f}; float energy{1.0f}; float patience{1.0f}; };
struct Visitor { std::uint32_t id{}; Vec2 position{}; TilePos tile{}; TilePos target{}; VisitorNeeds needs{}; VisitorState state{VisitorState::Entering}; std::vector<TilePos> route; std::size_t routeIndex{}; float decisionTimer{}; float stayTimer{}; float moveProgress{}; float speed{2.2f}; };
struct Building { BuildingType type{BuildingType::None}; TilePos tile{}; int rotation{}; };
struct DailyReport { int day{1}; int admitted{}; int departed{}; int foodSales{}; int revenue{}; int expenses{}; float satisfactionTotal{}; int satisfactionCount{}; };
}
