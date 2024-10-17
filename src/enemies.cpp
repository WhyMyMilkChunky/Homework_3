#include "enemies.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "tiles.h"

constexpr float ENEMY_RADIUS = 25.0f;

std::vector<Enemy> SpawnEnemies(const Vector2& initialPosition, int numberOfEnemies) {
    std::vector<Enemy> enemies;
    for (int i = 0; i < numberOfEnemies; ++i) {
        enemies.push_back({ initialPosition, {0, 0}, 100.0f, 100, 0 });
    }
    return enemies;
}

void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt) {
    for (Enemy& enemy : enemies) {
        int curr = enemy.currentWaypoint;
        int next = (curr + 1) % waypoints.size();
        Vector2 A = TileCenter(waypoints[curr]);
        Vector2 B = TileCenter(waypoints[next]);

        enemy.direction = Normalize(B - A);
        enemy.position = enemy.position + enemy.direction * enemy.speed * dt;

        if (CheckCollisionPointCircle(B, enemy.position, 10.0f)) {
            enemy.position = B;
            enemy.currentWaypoint = next;
        }
    }
}

void DrawEnemies(const std::vector<Enemy>& enemies) {
    for (const Enemy& enemy : enemies) {
        DrawCircleV(enemy.position, ENEMY_RADIUS, RED);
    }
}
