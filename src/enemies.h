#pragma once
#include "raylib.h"
#include <vector>

struct Enemy {
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    int currentWaypoint;
};

//init enemies
std::vector<Enemy> SpawnEnemies(const Vector2& initialPosition, int numberOfEnemies);

//update enemies
void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Vector2>& waypoints, float dt);

void DrawEnemies(const std::vector<Enemy>& enemies);