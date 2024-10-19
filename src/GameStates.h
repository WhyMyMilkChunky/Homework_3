#pragma once
#include "raylib.h"
//before round is begin
//during round is play
//game over is End
enum GameStates : int {
	BEGIN,
	PLAY,
	END
};
struct Game {
	Rectangle button;
	Color buttonColour;
	int gameState;
};
//Change button size and pos here
constexpr int BUTTON_POSX = 20;
constexpr int BUTTON_POSY = 20;
constexpr int BUTTON_WIDTH = 100;
constexpr int BUTTON_HEIGHT = 70;
Rectangle playButton = {
	BUTTON_POSX,
	BUTTON_POSY,
	BUTTON_WIDTH,
	BUTTON_HEIGHT
};
void ChangeGamemode(GameStates newGameState);
void DrawBegin(Game& game);
void DrawPlay(Game& game);
void DrawEnd(Game& game);
void UpdateBegin(Game& game);