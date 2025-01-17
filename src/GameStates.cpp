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

void ChangeGamemode(PlayStates newPlayState,Game& game)
{
    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), game.playButton);
    if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        game.playState = newPlayState;
}

void DrawBegin(Game& game)
{
    DrawRectangleRec(game.playButton, game.playButtonColour);
    int textx = game.playButton.width/2 - (MeasureText(game.buttonText, 30)) / 2;
    DrawText(game.buttonText,textx,game.playButton.y + 30, 30, WHITE);
}
void UpdateBegin(Button& button, Game& game) {
    // Add mouse-out vs mouse-over colour
    Color buttonColorOut = button.colour;
    //Color buttonColorIn = { button.colour .r /2, button.colour .b /2, button.colour .g /2, button.colour .a};
    Color buttonColorIn = ColorBrightness(button.colour,-0.4f);
    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), button.rec);
    button.buttonColour = mouseOver ? buttonColorIn : buttonColorOut;
    if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        game.gameState = button.stateChange;
}
void UpdateBegin(Game& game) {
    // Add mouse-out vs mouse-over colour
    Color buttonColorOut = game.playButtonColourOG;
    //Color buttonColorIn = { button.colour .r /2, button.colour .b /2, button.colour .g /2, button.colour .a};
    Color buttonColorIn = ColorBrightness(game.playButtonColourOG, -0.2f);
    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), game.playButton);
    game.playButtonColour = mouseOver ? buttonColorIn : buttonColorOut;

}
void UpdateBegin(std::vector<ToolbarButton>& buttons,Pen& pencil, Toolbar& toolbar)
{
    for (ToolbarButton& button : buttons) {
        // Add mouse-out vs mouse-over colour
        Color buttonColorOut = button.colour;
        //Color buttonColorIn = { button.colour .r /2, button.colour .b /2, button.colour .g /2, button.colour .a};
        Color buttonColorIn = ColorBrightness(button.colour, -0.4f);
        Color buttonColorSelected = ColorBrightness(button.colour, -0.7f);
        bool mouseOver = CheckCollisionPointRec(GetMousePosition(), button.rec);
        button.buttonColour = mouseOver ? buttonColorIn : buttonColorOut;

        //if has been selected and its not the <-- button, make button look selected;
        if (pencil.tileType == button.tileType && pencil.tileType != SWITCH)
            button.buttonColour = buttonColorSelected;

        //if selected change pencil to type
        if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            pencil.tileType = button.tileType;

            if (pencil.tileType == SWITCH) {
                toolbar = (toolbar == DECORTILES) ? BASICTILES : DECORTILES;
            }
        }

    }
}

//im realizing this is in an odd script as it should be in mainmenuui but like idk 
//trying to send in buttons

void DrawMainMenu(std::vector<Button>& Buttons)
{
    ClearBackground(BLACK);
    for (Button& button : Buttons) {
        DrawButton(button);
    }
    DisplayInstructions();
}
