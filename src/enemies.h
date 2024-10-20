#pragma once
#include "raylib.h"
#include <vector>

struct Cell;

enum class EnemyType {
    BASIC,
    WALLACE,
    JOSS,
    RUI,
    CONNOR
};

struct Enemy {
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    int currentWaypoint;
    EnemyType type;
};

void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt);
void DrawEnemies(const std::vector<Enemy>& enemies, Texture2D enemyTexture);
Enemy CreateEnemy(EnemyType type, Vector2 startPosition);
