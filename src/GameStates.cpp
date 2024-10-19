#include "GameStates.h"



//Change gamestate
GameStates currentGameState;
Rectangle playButton = {
    BUTTON_POSX,
    BUTTON_POSY,
    BUTTON_WIDTH,
    BUTTON_HEIGHT
};

void ChangeGamemode(GameStates newGameState)
{
	currentGameState = newGameState;
}

void DrawBegin(Game& game)
{
    DrawRectangleRec(game.button, game.buttonColour);
}

void DrawPlay(Game& game)
{
    DrawRectangleRec(game.button, Color{ 70, 128, 158, 255 });
}

void DrawEnd(Game& game)
{
    DrawRectangleRec(game.button, Color{ 50, 50, 50, 255 });
}

void UpdateBegin(Game& game) {
    // Add mouse-out vs mouse-over colour
    Color buttonColorOut = ORANGE;
    Color buttonColorIn = Color{ 205, 111, 0, 255 };
    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), game.button);
    game.buttonColour = mouseOver ? buttonColorIn : buttonColorOut;
  //  if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
   //     ++game.gameState %= 3;
}
