#include "raylib.h"
#include "Math.h"
#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include "enemies.h"

constexpr int SCREEN_SIZE = 800;
constexpr int TILE_SIZE = 40;
constexpr int TILE_COUNT = SCREEN_SIZE / TILE_SIZE;

constexpr float BULLET_RADIUS = 15.0f;
constexpr float ENEMY_RADIUS = 25.0f;

enum TileType : int
{
    GRASS,
    DIRT,
    WAYPOINT,
    COUNT
};
struct Cell
{
    int row;
    int col;
};

struct Turret {
    Vector2 position;
    float range;
    int damage;
    float firingRate;
};
Vector2 TileCenter(Cell cell)
{
    Vector2 pixel{ cell.col * TILE_SIZE, cell.row * TILE_SIZE };    // Top-left
    pixel = pixel + TILE_SIZE * 0.5f;   // Center
    return pixel;
}

constexpr std::array<Cell, 4> DIRECTIONS{ Cell{ -1, 0 }, Cell{ 1, 0 }, Cell{ 0, -1 }, Cell{ 0, 1 } };

inline bool InBounds(Cell cell, int rows = TILE_COUNT, int cols = TILE_COUNT)
{
    return cell.col >= 0 && cell.col < cols && cell.row >= 0 && cell.row < rows;
}

void DrawTile(int row, int col, Color color)
{
    DrawRectangle(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
}

void DrawTile(int row, int col, int tileType)
{
    assert(tileType >= 0 && tileType < COUNT);
    Color colours[COUNT]{ LIME, BEIGE, SKYBLUE };
    DrawTile(row, col, colours[tileType]);
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

struct Bullet
{
    Vector2 position{};
    Vector2 direction{};
    float time = 0.0f;
    bool enabled = true;
};

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
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 5
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, // 6
        { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0 }, // 7
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 8
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 9
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 10
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 11
        { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 12
        { 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 13
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 14
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 15
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 16
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0 }, // 17
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 18
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  // 19
    };
    
    // Automatic approach:
    std::vector<Cell> waypoints = FloodFill({ 0, 12 }, tiles, WAYPOINT);
    int curr = 0;
    int next = curr + 1;
    Vector2 enemyPosition = TileCenter(waypoints[curr]);
    float enemySpeed = 250.0f;
    bool atEnd = false;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    float bulletSpeed = 500.0f;
    float shootCurrent = 0.0f;
    float shootTotal = 0.25f;

    InitWindow(800, 800, "Game");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        spawnTimer += dt;
        Vector2 mouse = GetMousePosition();

        // Spawn new enemies at intervals
        if (enemies.size() < numberOfEnemies && spawnTimer >= spawnInterval) {
            
            Enemy newEnemy = {enemyPosition, {0, 0}, 100.0f, 100 };
            enemies.push_back(newEnemy);
            spawnTimer = 0.0f;
        }

        // Update all enemies
        for (Enemy& enemy : enemies) {
            // Set up current and next waypoints
            int nextWaypoint = enemy.currentWaypoint + 1;
            Vector2 A = TileCenter(waypoints[enemy.currentWaypoint]); // Current waypoint
            Vector2 B = TileCenter(waypoints[nextWaypoint]); // Next wypoint

            // Move enemy towards the next waypoint
            enemy.direction = Normalize(B - A);
            enemy.position = enemy.position + enemy.direction * enemy.speed * dt;
            
            //if enemy reached the next waypoint
            if (CheckCollisionPointCircle(B, enemy.position, 10.0f)) {
                //switch to the next one
                enemy.position = B;  // Snap
                enemy.currentWaypoint = nextWaypoint;         // Move to next waypoint
                nextWaypoint = (enemy.currentWaypoint + 1) % waypoints.size();
            }
        }

        // Draw tiles, bullets, etc.
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int row = 0; row < TILE_COUNT; row++) {
            for (int col = 0; col < TILE_COUNT; col++) {
                DrawTile(row, col, tiles[row][col]);
            }
        }

        // Draw enemies
        for (Enemy& enemy : enemies) {
            DrawCircleV(enemy.position, ENEMY_RADIUS, RED);
        }
        
        // Draw bullets
        for (const Bullet& bullet : bullets) {
            DrawCircleV(bullet.position, BULLET_RADIUS, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
