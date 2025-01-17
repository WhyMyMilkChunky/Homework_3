#pragma once
#include "tiles.h"
#include <vector>

constexpr int TOOLBAR_BUTTON_WIDTH = 160;
constexpr int TOOLBAR_BUTTON_HEIGHT = 100;
constexpr Color TOOLBAR_COLOR = BROWN;
enum Toolbar {
	BASICTILES,
	DECORTILES
};
//toolbar buttons
struct ToolbarButton {
	//current colour
	Color buttonColour;
	//Orginal Colour
	Color colour;
	//the number of the state this will change
	TileType tileType;
	Rectangle rec;
	const char* buttonText;
};
struct Pen {
	//change tile to this
	TileType tileType;
};
void CreateToolbarButton(float yOffset, TileType tileType, const char* buttonText, std::vector<ToolbarButton>& toolbarButtons);
//add ui toolbar that will hold the tile types to select
void DrawToolBar(ToolbarButton button);

//draw background and tiles
//if we wanted to just switch out the tile map, input different tiletex
void DrawBackground(int tiles[TILE_COUNT][TILE_COUNT],Texture2D tileTex);
Cell SelectCell();

void ChangeTile(Cell tilePos, TileType newTileType, int tiles[TILE_COUNT][TILE_COUNT]);

