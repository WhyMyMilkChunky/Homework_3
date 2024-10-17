#pragma once
#include "raylib.h"

constexpr int SCREEN_SIZE = 800;
constexpr int TILE_SIZE = 40;
constexpr int TILE_COUNT = SCREEN_SIZE / TILE_SIZE;
struct Cell {
    int row;
    int col;
};

Vector2 TileCenter(Cell cell);
