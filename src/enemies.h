#pragma once
#include "raylib.h"
#include <vector>

struct Cell;  // Forward declaration to avoid cyclic dependency

struct Enemy {
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    int currentWaypoint;
};

std::vector<Enemy> SpawnEnemies(const Vector2& initialPosition, int numberOfEnemies);
void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt);
void DrawEnemies(const std::vector<Enemy>& enemies);
