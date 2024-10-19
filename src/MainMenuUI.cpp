#include "MainMenuUI.h"
#include "GameStates.h"	


void CreateButton(int yOffset, const char* buttonText, Color buttonColour, int newState, std::vector<Button>& Buttons)
{
	//the button is CENTERED 
	int posX = (SCREEN_SIZEX / 2 - MENU_BUTTON_WIDTH/2);
	//i cant think anymore idk what this even means
	int posY = yOffset;
	//make background button
	Rectangle buttonRec = { posX,20.0f+yOffset,MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT };

	Button newButton;
	newButton.buttonColour = buttonColour;
	newButton.colour = buttonColour;
	newButton.rec = buttonRec;
	newButton.stateChange = newState;
	newButton.buttonText = buttonText;
	Buttons.push_back(newButton);
	
	//make rectangle at different x and y pos
	//do the hover over button thing ig
	//add text to each button,
	//mouse position click does certain function?
}
void DrawButton(Button button) {
	DrawRectangleRec(button.rec, button.buttonColour);

	// Text position is centered
	int textX = button.rec.x + (MENU_BUTTON_WIDTH / 2) - (MeasureText(button.buttonText, MENU_FONT_SIZE)) / 2;
	//that does the job
	int textY = button.rec.y;
	//Draw text over button
	DrawText(button.buttonText, textX, textY, 80, RAYWHITE);
}
