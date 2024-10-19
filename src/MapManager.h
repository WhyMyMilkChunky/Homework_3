#pragma once
#include <fstream>
#include <cstring>
#include "tiles.h"

class MapManager {
public:
    // t h e   c o n s t r u c t o r     :O
    MapManager(int tileCount);

    
    void SaveMap(int level, int map[TILE_COUNT][TILE_COUNT]);

    bool LoadMap(int level, int map[TILE_COUNT][TILE_COUNT]);

private:
    int tileCount;

    // generate filenames
    std::string GenerateFileName(int level);
};
