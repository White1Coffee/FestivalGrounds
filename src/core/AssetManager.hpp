#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>

namespace fg {
class AssetManager {
public:
    explicit AssetManager(SDL_Renderer* renderer) : renderer_(renderer) {}
    ~AssetManager() { clear(); }
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    SDL_Texture* loadBitmap(const std::string& key, const std::string& path);
    SDL_Texture* findTexture(const std::string& key) const;
    void clear();

private:
    SDL_Renderer* renderer_{}; // Renderer is owned by the game and outlives this manager.
    std::unordered_map<std::string, SDL_Texture*> textures_;
};
}
