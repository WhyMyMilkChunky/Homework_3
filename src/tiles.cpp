#include "tiles.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "MapMaker.h"

struct TileInfo {
    Rectangle source;
    TileType type;
};

TileInfo tileInfo[COUNT] = {
    { { 0, 0, 16, 16 }, GRASS},
    { { 16, 0, 16, 16 }, DIRT},
    { { 16, 0, 16, 16 }, WAYPOINT },
    { { 0, 16, 16, 16}, TURRET},
    { {16, 16, 16, 16},START},
    { {32, 0, 16, 16 },PUMPKIN },
    { {48, 0, 16, 16 },LOG},
    { {32, 16, 16, 16 },ROCK},
    { {48, 16, 16, 16}, PLANT},
    { {64, 0, 16, 16}, SPIKE}
};

//tile by specifying the tile type
void DrawTile(int row, int col, TileType tileType, Texture2D tex)
{
    TileInfo info = tileInfo[tileType];

    //destination rec
    Rectangle destRect = {static_cast<float>(col * TILE_SIZE),static_cast<float>(row * TILE_SIZE),TILE_SIZE, TILE_SIZE};
    //source rec
    Rectangle sourceRect = info.source;
    DrawTexturePro(tex, sourceRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

Vector2 TileCenter(Cell cell) {
    Vector2 pixel{ cell.col * TILE_SIZE, cell.row * TILE_SIZE };    // Top-left
    pixel = pixel + TILE_SIZE * 0.5f;   // Center
    return pixel;
}

bool IsCellValid(Cell selectedCell) {
    if (selectedCell.row <= TILE_COUNT && selectedCell.col <= TILE_COUNT)
        return true;
    else
    return true;
}