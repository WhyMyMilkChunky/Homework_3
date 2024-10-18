#include "turret.h"
#include "raylib.h"
#include "Math.h"
#include <cmath>


Turret CreateTurret(Vector2 position) {
    return { position, 200.0f, 25, 1.0f, 0.0f, 0.0f };
}

void DrawTurrets(const std::vector<Turret>& turrets, Texture2D turretTexture) {
    for (const Turret& turret : turrets) {
        
        DrawTexturePro(
            turretTexture,
            { 0, 0, (float)turretTexture.width, (float)turretTexture.height },
            { turret.pos.x, turret.pos.y, 40, 40 },
            { 20, 20 }, 
            turret.rotationAngle,
            WHITE
        );
    }
}

void UpdateTurrets(std::vector<Turret>& turrets, std::vector<Bullet>& bullets, const std::vector<Enemy>& enemies, float dt) {
    for (Turret& turret : turrets) {
        
        turret.cooldown -= dt;

        // Find the nearest enemy
        Enemy* target = FindNearestEnemy(turret, enemies);
        if (target) {
            
            Vector2 direction = Subtract(target->position, turret.pos);
            float angle = atan2f(direction.y, direction.x) * (180.0f / PI);
            turret.rotationAngle = angle; 

            
            if (turret.cooldown <= 0.0f) {
                Bullet bullet;
                bullet.pos = turret.pos;
                bullet.dir = Normalize(direction);
                bullet.enabled = true;
                bullets.push_back(bullet);

                turret.cooldown = turret.firingRate;
            }
        }
    }
}

Enemy* FindNearestEnemy(const Turret& turret, const std::vector<Enemy>& enemies) {
    Enemy* nearestEnemy = nullptr;
    float nearestDistance = turret.range;

    for (const Enemy& enemy : enemies) {
        float distance = Distance(turret.pos, enemy.position);
        if (distance < nearestDistance) {
            nearestEnemy = const_cast<Enemy*>(&enemy); // Return pointer to the nearest enemy
            nearestDistance = distance;
        }
    }

    return nearestEnemy;
}

