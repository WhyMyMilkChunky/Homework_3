#include "MapMaker.h"



void CreateToolbarButton(float yOffset,TileType tileType,const char* buttonText, std::vector<ToolbarButton>& toolbarButtons) {
    Rectangle ToolbarButtonRec = { 0 + yOffset,800, TOOLBAR_BUTTON_WIDTH, TOOLBAR_BUTTON_HEIGHT };
    ToolbarButton newButton;
    newButton.buttonColour = TOOLBAR_COLOR;
    newButton.colour = newButton.buttonColour;
    newButton.rec = ToolbarButtonRec;
    newButton.tileType = tileType;
    newButton.buttonText = buttonText;

    toolbarButtons.push_back(newButton);
}
void DrawToolBar()
{
    
    //technically not always grass, maybe change to feild or something
   // DrawRectangleRec(Grass.rec, Grass.buttonColour);
   // DrawRectangleRec(Dirt.rec, Dirt.buttonColour);
}

void DrawBackground(int tiles[TILE_COUNT][TILE_COUNT], Texture2D tileTex)
{
    
	ClearBackground(RAYWHITE);

    //tiles
   
    for (int row = 0; row < TILE_COUNT; row++) {
        for (int col = 0; col < TILE_COUNT; col++) {
            DrawTile(row, col, static_cast<TileType>(tiles[row][col]), tileTex);
        }
    }
}

void ChangeTile(Cell tilePos, TileType newTileType, int tiles[TILE_COUNT][TILE_COUNT]) {
    tiles[tilePos.row][tilePos.col] = newTileType;
}
Cell SelectCell() {
    int cellxCol = ceil(GetMousePosition().y / TILE_SIZE) - 1;
    int cellyRow = ceil(GetMousePosition().x / TILE_SIZE) - 1;
    Cell selectedCell = { cellxCol,cellyRow };
    return selectedCell;
}


