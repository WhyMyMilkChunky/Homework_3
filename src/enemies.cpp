#include "enemies.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "tiles.h"

constexpr float ENEMY_RADIUS = 25.0f;

void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt) {
    // Remove enemies that reach the last waypoint or have 0 health
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
            int curr = enemy.currentWaypoint;

            // Check if the enemy is at the last waypoint
            if (curr >= waypoints.size() - 1) {
                return true; // Remove enemy at the end of path
            }

            // Proceed to move the enemy
            int next = curr + 1;
            Vector2 A = TileCenter(waypoints[curr]);
            Vector2 B = TileCenter(waypoints[next]);

            enemy.direction = Normalize(B - A);
            enemy.position = enemy.position + enemy.direction * enemy.speed * dt;

            // If the enemy reaches the next waypoint
            if (CheckCollisionPointCircle(B, enemy.position, 10.0f)) {
                enemy.position = B;
                enemy.currentWaypoint = next;
            }

            // Remove enemies with zero health
            return enemy.health <= 0;
            }),
        enemies.end()
    );
}


void DrawEnemies(const std::vector<Enemy>& enemies) {
    for (const Enemy& enemy : enemies) {
        DrawCircleV(enemy.position, ENEMY_RADIUS, RED);
    }
}
