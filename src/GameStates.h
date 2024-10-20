#pragma once
#include "raylib.h"
#include"MainMenuUI.h"
#include "MapMaker.h"
//before first round is BEGINNEW, as in begin new game
//during round is play, button disabled
//BEGINROUND is state inbetween rounds
//GAMEOVER is after lose round, restart round
enum GameStates {
	MAINMENU,
	MAPMAKER,
	PLAYGAME,
	CREDITS
};
enum PlayStates : int {
	BEGINNEW,
	PLAY,
	BEGINROUND,
	GAMEOVER,
};
struct Game {
	// playstates 0-3
	int playState; 
	//GameStates 0,1,3
	int gameState; 
	//disable button play during the round
	bool isButtonEnabled;
	//colour for button during playstate
	Color playButtonColour;
	Color playButtonColourOG;
	Rectangle playButton;
};
//Change button size and pos here
constexpr int BUTTON_POSX = 0;
constexpr int BUTTON_POSY = 800;
constexpr int BUTTON_WIDTH = 160;
constexpr int BUTTON_HEIGHT = 200;
constexpr const char* BUTTON_TEXT_START = "Start Game";
constexpr const char* BUTTON_TEXT_NEXT = "Next Round";
constexpr const char* BUTTON_TEXT_NEW = "New Game";
constexpr const char* TEXT_OVER = "Game Over";

void ChangeGamemode(PlayStates newPlayState,Game& game);
void DrawBegin(Game& game);
void DrawPlay(Game& game);
void DrawEnd(Game& game);
//void UpdateBegin(Game& game);
void UpdateBegin(Button& button, Game& game);
void UpdateBegin(std::vector<ToolbarButton>& button, Pen& pencil);
void UpdateBegin(Game& game);
//mainmenu
void DrawMainMenu(std::vector<Button>& Buttons);

