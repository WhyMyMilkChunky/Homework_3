#pragma once
#include "raylib.h"
#include <vector>
//constexpr int SCREEN_SIZE = 800;
//I HAVE NO IDEA WHY BUTSCREEN SIZE STOPPED WORK WITH THE ERROR OF BEING INITIALIZED MULTIPLE TIMES BUT I SWEAR TO GODDDD IT WASNT ANYWHERE UGHHGRRJHGJKDGJNRDJKG
//ALL MY HOMIES HATE VISUAL STUDIES
constexpr int SCREEN_SIZEX = 800;
constexpr int SCREEN_SIZEY = 900;
constexpr int MENU_FONT_SIZE = 80;
constexpr int MENU_BUTTON_WIDTH = 600;
constexpr int MENU_BUTTON_HEIGHT = 100;
struct Button {
	//current colour
	Color buttonColour;
	//Orginal Colour
	Color colour;
		//the number of the state this will change
	int stateChange;
	Rectangle rec;
	const char* buttonText;
};

void CreateButton(int yOffset, const char* buttonText, Color buttonColour, int newState, std::vector<Button>& Buttons);
void DrawButton(Button button);