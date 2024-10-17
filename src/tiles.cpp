#include "tiles.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
Vector2 TileCenter(Cell cell) {
    Vector2 pixel{ cell.col * TILE_SIZE, cell.row * TILE_SIZE };    // Top-left
    pixel = pixel + TILE_SIZE * 0.5f;   // Center
    return pixel;
}