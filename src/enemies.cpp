#include "enemies.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "tiles.h"

constexpr float ENEMY_RADIUS = 25.0f;
constexpr int DAMAGE_PER_ENEMY = 10;

extern int currentHealth;


Enemy CreateEnemy(EnemyType type, Vector2 startPosition) {
    Enemy enemy;
    enemy.position = startPosition;
    enemy.currentWaypoint = 0;
    enemy.type = type;

    switch (type) {
    case EnemyType::BASIC:
        enemy.health = 100;
        enemy.speed = 100.0f;
        break;
    case EnemyType::WALLACE:
        enemy.health = 50;
        enemy.speed = 150.0f;
        break;
    case EnemyType::JOSS:
        enemy.health = 300;
        enemy.speed = 50.0f;
        break;
    case EnemyType::RUI:
        enemy.health = 20;
        enemy.speed = 200.0f;
        break;
    case EnemyType::CONNOR:
        enemy.health = 500;
        enemy.speed = 75.0f;
        break;
    }

    return enemy;
}


void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
            int curr = enemy.currentWaypoint;
            if (curr >= waypoints.size() - 1) {
                currentHealth -= DAMAGE_PER_ENEMY;
                if (currentHealth <= 0) {
                    currentHealth = 0;
                }
                return true;
            }

            int next = curr + 1;
            Vector2 A = TileCenter(waypoints[curr]);
            Vector2 B = TileCenter(waypoints[next]);
            enemy.direction = Normalize(B - A);
            enemy.position = enemy.position + enemy.direction * enemy.speed * dt;

            if (CheckCollisionPointCircle(B, enemy.position, 10.0f)) {
                enemy.position = B;
                enemy.currentWaypoint = next;
            }

            return enemy.health <= 0;
            }),
        enemies.end()
    );
}


void DrawEnemies(const std::vector<Enemy>& enemies, Texture2D enemyTexture) {
    for (const Enemy& enemy : enemies) {
        Color tint;

        //tint based on type
        switch (enemy.type) {
        case EnemyType::BASIC:
            tint = WHITE;
            break;
        case EnemyType::WALLACE:
            tint = BLUE;
            break;
        case EnemyType::JOSS:
            tint = GREEN;
            break;
        case EnemyType::RUI:
            tint = YELLOW;
            break;
        case EnemyType::CONNOR:
            tint = RED;
            break;
        }

        
        DrawTexturePro(enemyTexture,
            { 0, 0, (float)enemyTexture.width, (float)enemyTexture.height },
            { enemy.position.x, enemy.position.y, ENEMY_RADIUS * 2, ENEMY_RADIUS * 2 },
            { ENEMY_RADIUS, ENEMY_RADIUS },
            0,
            tint
        );
    }
}
