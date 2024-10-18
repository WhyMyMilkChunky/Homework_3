#pragma once
#include "raylib.h"
#include <vector>

struct Cell;

struct Enemy {
    Vector2 position;
    Vector2 direction;
    float speed;
    int health = 100;
    int currentWaypoint;
};
void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt);
void DrawEnemies(const std::vector<Enemy>& enemies);
