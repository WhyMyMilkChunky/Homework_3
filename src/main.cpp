#include "raylib.h"
#include "Math.h"
#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include "enemies.h"
#include "tiles.h"
#include "Turret.h"
#include "GameStates.h"
#include "ParticleSystem.h"
#include <iostream>
#include <fstream>
#include "MapMaker.h"
#include "MainMenuUI.h"
#include "MapManager.h"
#include <string>
#include "AudioManager.h"



//long main scripts give me anxiety so i refractored some of the code to different places
constexpr float BULLET_RADIUS = 6.0f;
constexpr float ENEMY_RADIUS = 25.0f;
constexpr float BULLET_SPEED = 600.0f;

extern Rectangle playButton;

//extern const char* BUTTON_TEXT; // maybe dont need?
extern ParticleSystem particleSys;
extern Button playAgain;
constexpr std::array<Cell, 4> DIRECTIONS{ Cell{ -1, 0 }, Cell{ 1, 0 }, Cell{ 0, -1 }, Cell{ 0, 1 } };
Pen pencil = { GRASS };
Pen inGamePencil = { TURRET };
Toolbar toolbar = BASICTILES;



WeatherType currentWeather = WeatherType::SUMMER;
Texture2D summerTileTex;
Texture2D winterTileTex;
void LoadWeatherTextures() {
    summerTileTex = LoadTexture("Assets/Textures/Tilemap1.png");
    winterTileTex = LoadTexture("Assets/Textures/Tilemap2.png");
}
void UnloadWeatherTextures() {
    UnloadTexture(summerTileTex);
    UnloadTexture(winterTileTex);
}

AudioManager audioManager;
std::vector<std::pair<std::string, std::string>> musicFiles = {
    {"playMusic", "Assets/Audio/music1.mp3"},
    {"readyMusic", "Assets/Audio/musicBuildStage.mp3"},
    {"gameOver", "Assets/Audio/musicGameOver.mp3" }
    
};

std::vector<std::pair<std::string, std::string>> sfxFiles = {
};

std::vector<std::pair<std::string, std::string>> spatialSfxFiles = {
    {"shot1", "Assets/Audio/shoot1.mp3"}
    
};
void LoadAllSounds(AudioManager& audioManager) {
    // Load music
    for (const auto& music : musicFiles) {
        audioManager.LoadMusic(music.first, music.second);
    }
    for (const auto& sfx : sfxFiles) {
        audioManager.LoadSFX(sfx.first, sfx.second);
    }
    for (const auto& sfx : spatialSfxFiles) {
        audioManager.LoadSpatialSFX("shot1", "Assets/Audio/shoot1.mp3", 10);
    }
}

int currentHealth = 100;
int maxHealth = 100;

int currentLevel = 1;

int totalEnemiesSpawned = 0;
int maxEnemiesPerLevel = 10;

void SaveCurrentLevel(int currentLevel) {
    std::ofstream file("current_level.txt");
    if (file.is_open()) {
        file << currentLevel;
        file.close();
        std::cout << "Current level saved as " << currentLevel << std::endl;
    }
    else {
        std::cerr << "Failed to open current_level.txt for writing" << std::endl;
    }
}
//This doesnt seem to do anything
//// Function to load the current level from a file
//int LoadCurrentLevel() {
//    std::ifstream file("current_level.txt");
//    int level = 1; // Default to level 1 if file doesn't exist
//    if (file.is_open()) {
//        file >> level;
//        file.close();
//        std::cout << "Loaded current level: " << level << std::endl;
//    }
//    else {
//        std::cerr << "Failed to open current_level.txt, defaulting to level 1" << std::endl;
//    }
//    return level;
//}
inline bool InBounds(Cell cell, int rows = TILE_COUNT, int cols = TILE_COUNT)
{
    return cell.col >= 0 && cell.col < cols && cell.row >= 0 && cell.row < rows;
}
Cell StartCell(int tiles[TILE_COUNT][TILE_COUNT]) {
    //search for startCell
    Cell startCell;
    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            // We don't want to search zero-tiles, so add them to closed!
            if (tiles[row][col] == START) {
                startCell = { row,col };
                return startCell;
            }

        }
    }
}
bool isNextLevel() {
    return true;
}
// Returns a collection of adjacent cells that match the search value.
std::vector<Cell> FloodFill(Cell start, int tiles[TILE_COUNT][TILE_COUNT], TileType searchValue)
{
    // "open" = "places we want to search", "closed" = "places we've already searched".
    std::vector<Cell> result;
    result.push_back(start);
    std::vector<Cell> open;
    bool closed[TILE_COUNT][TILE_COUNT];
    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            // We don't want to search zero-tiles, so add them to closed!
            closed[row][col] = tiles[row][col] == 0;
        }
    }

    // Add the starting cell to the exploration queue & search till there's nothing left!
    open.push_back(start);
    
    while (!open.empty())
    {
        // Remove from queue and prevent revisiting
        Cell cell = open.back();
        open.pop_back();
        closed[cell.row][cell.col] = true;
        closed[cell.row][cell.col] = true;

        // Add to result if explored cell has the desired value
        if (tiles[cell.row][cell.col] == searchValue)
            result.push_back(cell);

        // Search neighbours
        for (Cell dir : DIRECTIONS)
        {
            Cell adj = { cell.row + dir.row, cell.col + dir.col };
            if (InBounds(adj) &&
                !closed[adj.row][adj.col] &&
                tiles[adj.row][adj.col] > 0)
                open.push_back(adj);
        }
    }

    return result;
}

//yeah i didnt want to but the bullet logic here in main but ran outta time
void UpdateBullets(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, float dt) {
    
    for (auto& bullet : bullets) {
        if (!bullet.enabled) continue;

        //update bullet position
        bullet.pos.x += bullet.dir.x * BULLET_SPEED * dt;
        bullet.pos.y += bullet.dir.y * BULLET_SPEED * dt;

        //collision w enemies
        for (auto& enemy : enemies) {
            if (CheckCollisionPointCircle(bullet.pos, enemy.position, ENEMY_RADIUS)) {
                enemy.health -= 25;

                //kill bullet after hit
                bullet.enabled = false;

                if (enemy.health <= 0) {
                    enemy.health = 0;
                    //things below 0 give me anxiety
                }

                
                break;
            }
        }
    }

    //remove disabled bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.enabled; }),
        bullets.end()
    );
}
MapManager mapManager(TILE_COUNT);

float spawnInterval = 1.0f;
float spawnTimer = 0.0f;
int availableTurrets;
// Automatic approach:
std::vector<Cell> waypoints; 
int curr = 0;
int next = curr + 1;
Vector2 enemySpawnPosition;
float enemySpeed = 69.0f;
bool atEnd = false;

void InitalizeGameStuff(std::vector<Turret>& turrets, int tiles[TILE_COUNT][TILE_COUNT]) {
     spawnInterval = 1.0f;
     spawnTimer = 0.0f;

     turrets.clear();
   
     waypoints = FloodFill(StartCell(tiles), tiles, WAYPOINT);
    curr = 0;
    next= curr + 1;
     enemySpawnPosition = TileCenter(waypoints[curr]);
     enemySpeed = 69.0f;
     atEnd = false;

     std::srand(static_cast<unsigned>(std::time(0)));

    //this will need to be redone because we need to place the turrets during the game
    for (int row = 0; row < TILE_COUNT; row++)
    {
        for (int col = 0; col < TILE_COUNT; col++)
        {
            if (tiles[row][col] == TURRET)
            {
                //create and place turret at the center of the tile
                Vector2 turretPosition = TileCenter({ row, col });
                turrets.push_back(CreateTurret(turretPosition));
            }
        }
    }
    //change weather
    if (currentLevel % 2 == 0)
        currentWeather = WeatherType::WINTER;
    else
        currentWeather = WeatherType::SUMMER;
    particleSys.Clear();
    if (mapManager.LoadMap(currentLevel, tiles)) {
        std::cout << "Map loaded successfully!" << std::endl;

    }
    else {
        std::cout << "Failed to load map." << std::endl;
    }
};
int main()
{    
   // currentLevel = LoadCurrentLevel();
    int tiles[TILE_COUNT][TILE_COUNT];
    if (mapManager.LoadMap(currentLevel, tiles)) {
        std::cout << "Map loaded successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to load map." << std::endl;
    }
    
    Game game;
    game.playButton = playButton;
    game.playButtonColourOG = ORANGE;
    game.playButtonColour = game.playButtonColourOG;
    game.gameState = MAINMENU;
    game.playState = BEGINNEW;
    game.buttonText = "Start round";
    int numberOfEnemies = maxEnemiesPerLevel;
    
    LoadAllSounds(audioManager);

    //Creates vectors
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Turret> turrets;
    std::vector<ToolbarButton> toolbarButtons;
    std::vector<ToolbarButton> toolbarDecorButtons;
    std::vector<ToolbarButton> TurretButtons;
    std::vector<Button> Buttons;
    const std::array<EnemyType, 5> enemyTypes = {
    EnemyType::BASIC,
    EnemyType::WALLACE,
    EnemyType::JOSS,
    EnemyType::RUI,
    EnemyType::CONNOR
    };
    //Make ui buttons
    CreateButton(100, "Play", PURPLE, PLAYGAME,Buttons);
    CreateButton(250, "Map Maker", DARKPURPLE, MAPMAKER,Buttons);

    CreateToolbarButton(0, GRASS, "Grass",toolbarButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH, DIRT, "Dirt",toolbarButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH*2, WAYPOINT, "Waypoint",toolbarButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH*3, START, "StartCell",toolbarButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH*4,SWITCH, "--->",toolbarButtons);

    CreateToolbarButton(0,SWITCH, "<---", toolbarDecorButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH, PUMPKIN, "Pumpkin", toolbarDecorButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH*2, PLANT, "Plant", toolbarDecorButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH*3, ROCK, "Rock", toolbarDecorButtons);
    CreateToolbarButton(TOOLBAR_BUTTON_WIDTH*4, LOG, "Log", toolbarDecorButtons);

    CreateToolbarButton(BUTTON_WIDTH, TURRET, "Turret", TurretButtons);
    CreateToolbarButton(BUTTON_WIDTH*2-40, SPIKE, "Spikes", TurretButtons);
    CreateToolbarButton(BUTTON_WIDTH*3-80, SPIKE, "Spikes", TurretButtons);

   
    float shootCurrent = 0.0f;
    float shootTotal = 0.25f;

    int maxTurrets = 4;
    int placedTurrets = 0;
    

    InitalizeGameStuff(turrets, tiles);
    InitWindow(SCREEN_SIZEX, SCREEN_SIZEY, "Game");
    Texture2D tileTex = LoadTexture("Assets/Textures/tilemap1.png");
    Texture2D turretTex = LoadTexture("Assets/Textures/Turret_Top.png");
    Texture2D enemyTex = LoadTexture("Assets/Textures/Chomp.png");
    LoadWeatherTextures();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        audioManager.Update({ SCREEN_SIZEX * 0.5 , SCREEN_SIZEY* 0.5 });
        std::string levelText = "Current Level: " + std::to_string(currentLevel);
        if (IsKeyReleased(KEY_SPACE))
        {
            game.gameState = MAINMENU;
        }
        switch (game.gameState) {
        case MAINMENU:
            for (Button& button : Buttons) {
                UpdateBegin(button,game);
            }
            break;
        case MAPMAKER:
            if (IsKeyPressed(KEY_S)) {
                mapManager.SaveMap(currentLevel, tiles);
            }
            if (IsKeyPressed(KEY_L)) {
                mapManager.LoadMap(currentLevel, tiles);
            }
            if (IsKeyPressed(KEY_PAGE_UP)) {
                currentLevel++;
                mapManager.LoadMap(currentLevel, tiles);
            }
            if (IsKeyPressed(KEY_PAGE_DOWN) && currentLevel > 1) {
                currentLevel--;
                mapManager.LoadMap(currentLevel, tiles);
            }
            
            DrawText(levelText.c_str(), 10, 10, 20, BLACK);
            switch (toolbar) {
            case BASICTILES:
                UpdateBegin(toolbarButtons, pencil, toolbar);
                break;
            case DECORTILES:
                UpdateBegin(toolbarDecorButtons, pencil, toolbar);
                break;
            }
            //check if pencil is a tiletype and if the click was on the map
            if (pencil.tileType < SWITCH) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    if (IsCellValid(SelectCell()))
                        ChangeTile(SelectCell(), pencil.tileType, tiles);
            }
            break;
        case PLAYGAME:

            UpdateBegin(game);
         
            //place turrets
            Vector2 mouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Cell selectedCell = SelectCell();

                
                if (placedTurrets < maxTurrets && InBounds(selectedCell) && tiles[selectedCell.row][selectedCell.col] == GRASS) {
                    ChangeTile(selectedCell, TURRET, tiles);

                    Vector2 turretPosition = TileCenter(selectedCell);
                    turrets.push_back(CreateTurret(turretPosition));

                    placedTurrets++;
                }
                else if (placedTurrets >= maxTurrets) {
                    std::cout << "Turret limit reached!" << std::endl;
                }
            }
            //update all enemies
            UpdateEnemies(enemies, waypoints, dt);
            UpdateTurrets(turrets, bullets, enemies, dt, audioManager);
            UpdateBullets(bullets, enemies, dt);

            particleSys.Update(dt);
            //death switch state
            if (currentHealth <= 0) {
                game.playState = GAMEOVER;
            }
            
            switch (game.playState)
            {
            case BEGINNEW:
                //setup before game
                audioManager.PlayMusic("readyMusic");
                game.isButtonEnabled = true;
                ChangeGamemode(PLAY, game);
                break;

            case PLAY:
                //during game
                game.isButtonEnabled = false;
                audioManager.PlayMusic("playMusic");
                spawnTimer += dt;
                if (totalEnemiesSpawned >= maxEnemiesPerLevel && enemies.empty()) {
                    bullets.clear();
                    //check if there is a next level
                    if (isNextLevel())
                        currentLevel++;
                    else
                        currentLevel = 0;
                        mapManager.LoadMap(currentLevel, tiles);

                    InitalizeGameStuff(turrets, tiles);
                    game.playState = BEGINNEW;
                    totalEnemiesSpawned = 0;
                    maxEnemiesPerLevel += 10;
                    maxTurrets += 2;
                    placedTurrets = 0;
                }
                //spawns
                if (totalEnemiesSpawned < maxEnemiesPerLevel && spawnTimer >= spawnInterval) {
                    EnemyType randomType = enemyTypes[std::rand() % enemyTypes.size()];
                    Enemy newEnemy = CreateEnemy(randomType, enemySpawnPosition);
                    enemies.push_back(newEnemy);
                    spawnTimer = 0.0f;
                    totalEnemiesSpawned++;
                }

                break;

            case GAMEOVER:
                //when run out of health
                bullets.clear();
                enemies.clear();
                turrets.clear();
                //PLAY SOUND
                audioManager.PlayMusic("gameOver");
                game.gameState = CREDITS;
                game.playState = BEGINNEW;
                break;
            }
            break;
        case CREDITS:           
            UpdateBegin(playAgain, game);
            //reset evewrything
            currentHealth = maxHealth;
            currentLevel = 1;
            break;
        }
       

        BeginDrawing();
        switch (game.gameState) {
        case MAINMENU:
            DrawMainMenu(Buttons);
                break;

        case MAPMAKER:
            
            DrawBackground(tiles, summerTileTex);
            switch (toolbar) {
            case BASICTILES:

                for (ToolbarButton& button : toolbarButtons) {

                    DrawToolBar(button);
                }
                break;
            case DECORTILES:
                for (ToolbarButton& button : toolbarDecorButtons) {

                    DrawToolBar(button);
                }
                break;
            }
            DrawText(levelText.c_str(), 10, 10, 20, BLACK);
                    break;
        case PLAYGAME :
            availableTurrets = maxTurrets - placedTurrets;
            //Make snow
            if (currentWeather == WeatherType::WINTER) {
                particleSys.CreateSnow(5, Random(1.0f, 5.0f), 2.0f, 50.0f, 90.0f, WHITE, 1.0f, GetScreenWidth(), GetScreenHeight());
            }
            
            ClearBackground(BLACK);
            //tiles
            for (int row = 0; row < TILE_COUNT; row++) {
                for (int col = 0; col < TILE_COUNT; col++) {
                    switch (currentWeather) {
                    case WeatherType::SUMMER:
                        DrawTile(row, col, static_cast<TileType>(tiles[row][col]), summerTileTex);

                        break;
                    case WeatherType::WINTER:
                        DrawTile(row, col, static_cast<TileType>(tiles[row][col]), winterTileTex);

                        break;
                    }
                }
            }

            //draw enemies
            DrawEnemies(enemies, enemyTex);
            //Draw bullets
            for (const Bullet& bullet : bullets) {
                DrawCircleV(bullet.pos, BULLET_RADIUS, RED);
            }
            //draw turrets
            DrawTurrets(turrets, turretTex);
            //add particles to turrets
            particleSys.Draw();
            
            DrawText(("Turrets Left: " + std::to_string(availableTurrets)).c_str(), 10, 40, 20, BLACK);
            //this will draw the in game playstates button
            DrawHealthBar(currentHealth, maxHealth);
            //draw in game tool bar
            for (ToolbarButton& button : TurretButtons) {

                DrawToolBar(button);
            }
            switch (game.playState)
            {
            case BEGINNEW:
                //Draws button for colour hover thing
                DrawBegin(game);
                break;

            case PLAY:

                break;

            case GAMEOVER:

                break;
            }
            break;
        case CREDITS:
            DrawGameOver();
            break;
        }
        EndDrawing();
    }
    UnloadTexture(tileTex);
    UnloadWeatherTextures();
    CloseWindow();
    enemies.clear();
    return 0;
}