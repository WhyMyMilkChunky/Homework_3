#include "enemies.h"
#include <algorithm>
#include "Math.h"

constexpr float ENEMY_RADIUS = 25.0f;

std::vector<Enemy> SpawnEnemies(const Vector2& initialPosition, int numberOfEnemies) {
    std::vector<Enemy> enemies;

    for (int i = 0; i < numberOfEnemies; ++i) {
        Enemy newEnemy = { initialPosition, {0, 0}, 100.0f, 100 };
        enemies.push_back(newEnemy);
    }

    return enemies;
}
void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Vector2>& waypoints, float dt) {
    static int curr = 0;
    static int next = curr + 1;

    for (Enemy& enemy : enemies) {
        Vector2 A = waypoints[curr]; // Current waypoint
        Vector2 B = waypoints[next]; // Next waypoint

        // Move enemy towards the next waypoint
        enemy.direction = Normalize(B - A);
        enemy.position = enemy.position + enemy.direction * enemy.speed * dt;

        // If enemy reached the next waypoint
        if (CheckCollisionPointCircle(B, enemy.position, 10.0f)) {
            enemy.position = B;  // Snap
            curr = next;         // Move to next waypoint
            next = (curr + 1) % waypoints.size();
        }
    }
}
void DrawEnemies(const std::vector<Enemy>& enemies) {
    for (const Enemy& enemy : enemies) {
        DrawCircleV(enemy.position, ENEMY_RADIUS, RED);
    }
}