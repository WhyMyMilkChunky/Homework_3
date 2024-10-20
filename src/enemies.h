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
    int maxHealth;
    int currentWaypoint;
    EnemyType type;
};

void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt);
void DrawEnemies(const std::vector<Enemy>& enemies, Texture2D enemyTexture);
void DrawEnemyHealthBar(Vector2 enemyPosition, int currentHealth, int maxHealth, float enemyRadius);
Enemy CreateEnemy(EnemyType type, Vector2 startPosition);
