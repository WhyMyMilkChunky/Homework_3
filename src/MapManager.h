#pragma once
#include <fstream>
#include <cstring>
#include "tiles.h"

class MapManager {
public:
    // Constructor
    MapManager(int tileCount);

    // Save the map to a binary file
    void SaveMap(const std::string& filePath, int map[TILE_COUNT][TILE_COUNT]);

    // Load the map from a binary file
    bool LoadMap(const std::string& filePath, int map[TILE_COUNT][TILE_COUNT]);

private:
    int tileCount;
};
