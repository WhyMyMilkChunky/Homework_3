#include "MainMenuUI.h"
#include "GameStates.h"	
#include <string>


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
void DrawHealthBar(int currentHealth, int maxHealth)
{
	//centered horizontally
	int posX = (SCREEN_SIZEX / 2) - (HEALTH_BAR_WIDTH / 2);
	int posY = SCREEN_TOP_MARGIN;

	//width of the inner health bar
	float healthPercentage = (float)currentHealth / (float)maxHealth;
	int innerBarWidth = (int)(HEALTH_BAR_WIDTH * healthPercentage);

	//da outline
	Rectangle outerRect = { (float)posX - HEALTH_BAR_OUTLINE_THICKNESS, (float)posY - HEALTH_BAR_OUTLINE_THICKNESS,
							(float)HEALTH_BAR_WIDTH + 2 * HEALTH_BAR_OUTLINE_THICKNESS,
							(float)HEALTH_BAR_HEIGHT + 2 * HEALTH_BAR_OUTLINE_THICKNESS };

	//actual helth bar
	Rectangle innerRect = { (float)posX, (float)posY, (float)innerBarWidth, (float)HEALTH_BAR_HEIGHT };

	DrawRectangleRec(outerRect, BLACK);
	DrawRectangleRec(innerRect, RED);

	//display curr helth/max health
	std::string healthText = std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
	int textWidth = MeasureText(healthText.c_str(), 20);
	int textPosX = (SCREEN_SIZEX / 2) - (textWidth / 2);
	int textPosY = posY + (HEALTH_BAR_HEIGHT / 2) - 10;  // vertically center txt
	DrawText(healthText.c_str(), textPosX, textPosY, 20, WHITE);
}
