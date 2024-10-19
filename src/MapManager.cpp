#include "MapManager.h"
#include <iostream>

MapManager::MapManager(int tileCount) : tileCount(tileCount) {}

void MapManager::SaveMap(const std::string& filePath, int map[TILE_COUNT][TILE_COUNT]) {
    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(map), sizeof(int) * tileCount * tileCount);
        file.close();
        std::cout << "Map saved successfully to " << filePath << std::endl;
    }
    else {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
    }
}

bool MapManager::LoadMap(const std::string& filePath, int map[TILE_COUNT][TILE_COUNT]) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(map), sizeof(int) * tileCount * tileCount);
        file.close();
        std::cout << "Map loaded successfully from " << filePath << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to open file for reading: " << filePath << std::endl;
        return false;
    }
}
