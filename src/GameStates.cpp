#include "GameStates.h"
#include "MainMenuUI.h"



//Change gamestate
PlayStates currentPlayState;
Rectangle playButton = {
    BUTTON_POSX,
    BUTTON_POSY,
    BUTTON_WIDTH,
    BUTTON_HEIGHT
};

void ChangeGamemode(PlayStates newPlayState)
{
	currentPlayState = newPlayState;
}

void DrawBegin(Game& game)
{
    DrawRectangleRec(game.playButton, game.playButtonColour);
}

void DrawPlay(Game& game)
{
    DrawRectangleRec(game.playButton, Color{ 70, 128, 158, 255 });
}

void DrawEnd(Game& game)
{
    DrawRectangleRec(game.playButton, Color{ 50, 50, 50, 255 });
}

//void UpdateBegin(Game& game) {
//    // Add mouse-out vs mouse-over colour
//    Color buttonColorOut = ORANGE;
//    Color buttonColorIn = Color{ 205, 111, 0, 255 };
//    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), game.playButton);
//    game.playButtonColour = mouseOver ? buttonColorIn : buttonColorOut;
//  //  if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
//   //     ++game.gameState %= 3;
//}       
void UpdateBegin(Button& button, Game& game) {
    // Add mouse-out vs mouse-over colour
    Color buttonColorOut = button.colour;
    //Color buttonColorIn = { button.colour .r /2, button.colour .b /2, button.colour .g /2, button.colour .a};
    Color buttonColorIn = ColorBrightness(button.colour,-0.4);
    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), button.rec);
    button.buttonColour = mouseOver ? buttonColorIn : buttonColorOut;
    if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        game.gameState = button.stateChange;
}
//trying to send in buttons
void DrawMainMenu(std::vector<Button>& Buttons)
{
    ClearBackground(BLACK);
    for (Button& button : Buttons) {
        DrawButton(button);
    }
}
