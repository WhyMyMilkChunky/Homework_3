#pragma once
#include "raylib.h"
#include <vector>
#include <string>
constexpr float ENEMY_RADIUS = 25.0f;
constexpr int DAMAGE_PER_ENEMY = 10;
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
    int pointWorth;
    float speed;
    int health;
    int maxHealth;
    int currentWaypoint;
    EnemyType type;
};
struct FloatingText {
    Vector2 pos;
    std::string text;
    float alpha;
    float lifetime;
};

void UpdateFloatingTexts(std::vector<FloatingText>& floatingTexts, float dt);
void DrawFloatingTexts(const std::vector<FloatingText>& floatingTexts);
void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt, std::vector<FloatingText>& floatingTexts);
void DrawEnemies(const std::vector<Enemy>& enemies, Texture2D enemyTexture);
void DrawEnemyHealthBar(Vector2 enemyPosition, int currentHealth, int maxHealth, float enemyRadius);
Enemy CreateEnemy(EnemyType type, Vector2 startPosition);
