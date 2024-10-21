#include "tiles.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "MapMaker.h"

struct TileInfo {
    Rectangle source;
    TileType type;
    Toolbar setType;
};

TileInfo tileInfo[COUNT] = {
    { { 0, 0, 16, 16 }, GRASS, BASICTILES },
    { { 16, 0, 16, 16 }, DIRT ,BASICTILES},
    { { 16, 0, 16, 16 }, WAYPOINT,BASICTILES },
    { { 0, 16, 16, 16}, TURRET ,BASICTILES},
    { {16, 16, 16, 16},START,BASICTILES},
    { {32, 0, 16, 16 },PUMPKIN,DECORTILES },
    { {48, 0, 16, 16 },LOG,DECORTILES},
    { {32, 16, 16, 16 },ROCK,DECORTILES},
    { {48, 16, 16, 16}, PLANT,DECORTILES}
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