#include "raylib.h"
#include "Math.h"
#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include "enemies.h"
#include "tiles.h"
#include "Turret.h"


constexpr float BULLET_RADIUS = 6.0f;
constexpr float ENEMY_RADIUS = 25.0f;
constexpr float BULLET_SPEED = 600.0f;

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
void UpdateBullets(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, float dt) {
    
    for (auto& bullet : bullets) {
        if (!bullet.enabled) continue;

        //update bullet position
        bullet.pos.x += bullet.dir.x * BULLET_SPEED * dt;
        bullet.pos.y += bullet.dir.y * BULLET_SPEED * dt;

        //collisions with enemies
        for (auto& enemy : enemies) {
            if (CheckCollisionPointCircle(bullet.pos, enemy.position, ENEMY_RADIUS)) {
                // Subtract health from the enemy
                enemy.health -= 25;

                //kill bullet after hit
                bullet.enabled = false;

                //is enemy dead
                if (enemy.health <= 0) {
                    enemy.health = 0;
                }

                
                break;
            }
        }
    }

    // Remove disabled bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.enabled; }),
        bullets.end()
    );
}
int main()
{
    const int numberOfEnemies = 10;
    float spawnInterval = 1.0f;
    float spawnTimer = 0.0f;

    int tiles[TILE_COUNT][TILE_COUNT]
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
        DrawTurrets(turrets, turretTex);

        EndDrawing();
    }
    UnloadTexture(tileTex);
    CloseWindow();
    return 0;
}
