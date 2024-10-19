#pragma once
#include "raylib.h"
#include "enemies.h"

//i wanted enemies in another script and found out i had to refractor the tiles too
constexpr int SCREEN_SIZE = 800;
constexpr int TILE_SIZE = 40;
constexpr int TILE_COUNT = SCREEN_SIZE / TILE_SIZE;


enum TileType : int
{
    GRASS,
    DIRT,
    WAYPOINT,
    TURRET,
    START,
    COUNT
    
};
void DrawTile(int row, int col, TileType tileType, Texture2D tex);
struct Cell {
    int row;
    int col;
};

Vector2 TileCenter(Cell cell);


