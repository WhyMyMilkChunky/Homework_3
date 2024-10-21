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
void DrawToolBar(ToolbarButton button)
{
    //draw button rectangle
    DrawRectangleRec(button.rec, button.buttonColour);
    //center text
    int textX = button.rec.x + (TOOLBAR_BUTTON_WIDTH / 2) - (MeasureText(button.buttonText, 30)) / 2;
    int textY = button.rec.y+ (TOOLBAR_BUTTON_HEIGHT/2 - 15);

    DrawText(button.buttonText, textX, textY, 30, RAYWHITE);
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



