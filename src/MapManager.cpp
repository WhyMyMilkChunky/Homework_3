#include "MapManager.h"
#include <iostream>
#include <fstream>
#include <string>

MapManager::MapManager(int tileCount) : tileCount(tileCount) {}

//generate le file name (game takes place in france)
std::string MapManager::GenerateFileName(int level) {
    return "map" + std::to_string(level) + ".bin";
}

//save 
void MapManager::SaveMap(int level, int map[TILE_COUNT][TILE_COUNT]) {
    std::string filePath = GenerateFileName(level);
    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<char*>(map), sizeof(int) * tileCount * tileCount);
        file.close();
        std::cout << "map saved successfully to " << filePath << std::endl;
    }
    else {
        std::cerr << "UH OH failed to open file for writing: " << filePath << std::endl;
    }
}

//load
bool MapManager::LoadMap(int level, int map[TILE_COUNT][TILE_COUNT]) {
    std::string filePath = GenerateFileName(level);
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(map), sizeof(int) * tileCount * tileCount);
        file.close();
        std::cout << "map loaded successfully from " << filePath << std::endl;
        return true;
    }
    else {
        std::cerr << "OH SHIT!! failed to open file for reading: " << filePath << std::endl;
        return false;
    }
}
