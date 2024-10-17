#include "tiles.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include <cassert>

// Define the colors for each tile type
Color tileColors[COUNT]{ LIME, BEIGE, SKYBLUE };

// Draw a tile by specifying the color directly
void DrawTile(int row, int col, Color color)
{
    DrawRectangle(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
}

// Draw a tile by specifying the tile type
void DrawTile(int row, int col, TileType tileType)
{
    assert(tileType >= 0 && tileType < COUNT);
    DrawTile(row, col, tileColors[tileType]);
}


Vector2 TileCenter(Cell cell) {
    Vector2 pixel{ cell.col * TILE_SIZE, cell.row * TILE_SIZE };    // Top-left
    pixel = pixel + TILE_SIZE * 0.5f;   // Center
    return pixel;
}