#include "core/AssetManager.hpp"
#include "core/Logger.hpp"

namespace fg {
SDL_Texture* AssetManager::loadBitmap(const std::string& key, const std::string& path) {
    if (auto* cached = findTexture(key)) return cached;
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface) {
        Logger::write(LogLevel::Error, SDL_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        Logger::write(LogLevel::Error, SDL_GetError());
        return nullptr;
    }
    textures_.emplace(key, texture);
    return texture;
}

SDL_Texture* AssetManager::findTexture(const std::string& key) const {
    const auto found = textures_.find(key);
    return found == textures_.end() ? nullptr : found->second;
}

void AssetManager::clear() {
    for (auto& [key, texture] : textures_) {
        (void)key;
        SDL_DestroyTexture(texture);
    }
    textures_.clear();
}
}
