#include "enemies.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "tiles.h"

constexpr float ENEMY_RADIUS = 25.0f;
constexpr int DAMAGE_PER_ENEMY = 10;

extern int currentHealth;
//i pretty much just moved your code here
void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt) {

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
            int curr = enemy.currentWaypoint;    
            //poof, gone!
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
            return enemy.health <= 0;//permanent rest
            }),
        enemies.end()
    );
}
void DrawEnemies(const std::vector<Enemy>& enemies, Texture2D enemyTexture) {
    for (const Enemy& enemy : enemies) {
       // DrawCircleV(enemy.position, ENEMY_RADIUS, RED);
        DrawTexturePro(enemyTexture,
            { 0, 0, (float)enemyTexture.width, (float)enemyTexture.height },
            { enemy.position.x, enemy.position.y, ENEMY_RADIUS*2, ENEMY_RADIUS*2 },
            { ENEMY_RADIUS,ENEMY_RADIUS },
            0,
            WHITE
        );
    }
}
