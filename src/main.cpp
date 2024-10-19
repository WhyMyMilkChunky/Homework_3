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


//long main scripts give me anxiety so i refractored some of the code to different places
constexpr float BULLET_RADIUS = 6.0f;
constexpr float ENEMY_RADIUS = 25.0f;
constexpr float BULLET_SPEED = 600.0f;
extern Rectangle playButton;
//extern const char* BUTTON_TEXT; // maybe dont need?
extern ParticleSystem particleSys;
constexpr std::array<Cell, 4> DIRECTIONS{ Cell{ -1, 0 }, Cell{ 1, 0 }, Cell{ 0, -1 }, Cell{ 0, 1 } };

inline bool InBounds(Cell cell, int rows = TILE_COUNT, int cols = TILE_COUNT)
{
    return cell.col >= 0 && cell.col < cols && cell.row >= 0 && cell.row < rows;
}

// Returns a collection of adjacent cells that match the search value.
std::vector<Cell> FloodFill(Cell start, int tiles[TILE_COUNT][TILE_COUNT], TileType searchValue)
{
    // "open" = "places we want to search", "closed" = "places we've already searched".
    std::vector<Cell> result;
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
int main()
{
    int map[TILE_COUNT][TILE_COUNT]
    {
        //col:0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19    row:
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, // 0
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 1
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 2
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 3
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 4
            { 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 }, // 5
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 6
            { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0 }, // 7
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 8
            { 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 9
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 10
            { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 11
            { 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 12
            { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 13
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 14
            { 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 15
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 16
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 17
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 18
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  // 19
    };

    int tiles[TILE_COUNT][TILE_COUNT];

    //create a binary file 
    std::ofstream myfile;
    myfile.open("example.bin", std::ios::out | std::ios::app | std::ios::binary);
    myfile.write(reinterpret_cast<char*>(map), sizeof(map));
    myfile.close();
   
    //getting the binary file and copying the memory
    std::streampos size;
    char* memblock;
    std::ifstream file("example.bin", std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();
        

        int copy[TILE_COUNT][TILE_COUNT];
        std::memcpy(copy, memblock, sizeof(copy));
        for (int i = 0; i < TILE_COUNT; ++i) {
            for (int j = 0; j < TILE_COUNT; ++j) {
                tiles[i][j] = copy[i][j];
            }
        }
        delete[] memblock;
        printf("the entire file content is in memory");
   
      //  //make a copy of last file using memory
      //  std::ofstream myCopyFile;
      //  myCopyFile.open("exampleCopy.bin", std::ios::out | std::ios::app | std::ios::binary);
      //  if (myCopyFile.is_open()) {
      //      myCopyFile.write(memblock, size); // Write binary data
      //      myCopyFile.close();
      //  }
      //  else {
      //      std::cout << "Unable to open file for copying." << std::endl;
      //  }
      //  delete[] memblock;
    }

    Game game;
    game.button = playButton;
    game.buttonColour = ORANGE;
    game.gameState = BEGINNEW;
    const int numberOfEnemies = 10;
    float spawnInterval = 1.0f;
    float spawnTimer = 0.0f;

    int tilesOLD[TILE_COUNT][TILE_COUNT]
    {
    //col:0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19    row:
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 }, // 0
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 1
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 2
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 3
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 4
        { 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 5
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 6
        { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0 }, // 7
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 8
        { 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 9
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 10
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 11
        { 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 12
        { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 13
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 14
        { 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 15
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 16
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 17
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 18
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  // 19
    };

    // Automatic approach:
    std::vector<Cell> waypoints = FloodFill({ 0, 12 }, tiles, WAYPOINT);
    int curr = 0;
    int next = curr + 1;
    Vector2 enemySpawnPosition = TileCenter(waypoints[curr]);
    float enemySpeed = 69.0f;
    bool atEnd = false;
    
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Turret> turrets;
   
    for (int row = 0; row < TILE_COUNT; row++) {
        for (int col = 0; col < TILE_COUNT; col++) {
            if (tiles[row][col] == TURRET) {
                //create and place turret at the center of the tile
                Vector2 turretPosition = TileCenter({ row, col });
                turrets.push_back(CreateTurret(turretPosition));
            }
        }
    }
    float bulletSpeed = 500.0f;
    float shootCurrent = 0.0f;
    float shootTotal = 0.25f;

    InitWindow(800, 800, "Game");
    Texture2D tileTex = LoadTexture("Assets/Textures/tilemap1.png");
    Texture2D turretTex = LoadTexture("Assets/Textures/Turret_Top.png");
    Texture2D enemyTex = LoadTexture("Assets/Textures/Chomp.png");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        spawnTimer += dt;
        Vector2 mouse = GetMousePosition();

        //spawns
        if (enemies.size() < numberOfEnemies && spawnTimer >= spawnInterval) {
            
            Enemy newEnemy = {enemySpawnPosition, {0, 0}, enemySpeed};
            enemies.push_back(newEnemy);
            spawnTimer = 0.0f;
        }

        //update all enemies
        UpdateEnemies(enemies, waypoints, dt);
        UpdateTurrets(turrets, bullets, enemies, dt);
        UpdateBullets(bullets, enemies, dt);
        particleSys.Update(dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //tiles
        for (int row = 0; row < TILE_COUNT; row++) {
            for (int col = 0; col < TILE_COUNT; col++) {
                DrawTile(row, col, static_cast<TileType>(tiles[row][col]), tileTex);
            }
        }
        for (const Bullet& bullet : bullets) {
            DrawCircleV(bullet.pos, BULLET_RADIUS, RED);
        }
        //draw enemies
        DrawEnemies(enemies,enemyTex);
        
        //draw bullets
        for (const Bullet& bullet : bullets) {
            DrawCircleV(bullet.pos, BULLET_RADIUS, RED);
        }
        particleSys.Draw();
        DrawTurrets(turrets, turretTex);
        

        switch (game.gameState)
        {
        case BEGINNEW:
            DrawBegin(game);
            break;

        case PLAY:
            DrawPlay(game);
            break;

        case GAMEOVER:
            DrawEnd(game);
            break;
        }
        EndDrawing();
    }
    UnloadTexture(tileTex);
    CloseWindow();
    return 0;
}
