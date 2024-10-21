#include "turret.h"
#include "raylib.h"
#include "Math.h"
#include <cmath>
#include "ParticleSystem.h"
#include "MainMenuUI.h"
#include "AudioManager.h"
#include "tiles.h"
#include "enemies.h"

ParticleSystem particleSys;
Turret CreateTurret(Vector2 position) {
    return { position, 200.0f, 25, 1.0f, 0.0f, 0.0f };
}
void DrawTurrets(std::vector<Turret>& turrets, Texture2D turretTexture) {
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

void UpdateTurrets(std::vector<Turret>& turrets, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, float dt, AudioManager& audioManager) {
    for (Turret& turret : turrets) {

        turret.cooldown -= dt;

        // Find the nearest enemy
        Enemy* target = FindNearestEnemy(turret, enemies);
        if (target) {
            Vector2 direction = Subtract(target->position, turret.pos);
            float angle = atan2f(direction.y, direction.x) * (180.0f / PI);
            turret.rotationAngle = angle; //awkwardly snap the turret to point at its new "friend"

            if (turret.cooldown <= 0.0f) {
                Bullet bullet;
                bullet.pos = turret.pos;
                bullet.dir = Normalize(direction);
                bullet.enabled = true;
                bullets.push_back(bullet);

                // Create muzzle flash particle effect
                particleSys.CreateMuzzleFlash(turret.pos, bullet.dir, 13, 2, 3, YELLOW, 0.2F, 150, 0.2F);
                audioManager.PlaySpatialSFX("shot1", {SCREEN_SIZEY * 0.5,SCREEN_SIZEX * 0.5, }, turret.pos, 1, 800);

                turret.cooldown = turret.firingRate; // Reset cooldown before firing again
            }
        }
    }
}

Enemy* FindNearestEnemy(Turret& turret, std::vector<Enemy>& enemies) {
    Enemy* nearestEnemy = nullptr;
    float nearestDistance = turret.range;

    for (Enemy& enemy : enemies) {
        float distance = Distance(turret.pos, enemy.position);
        if (distance < nearestDistance) {
            nearestEnemy = &enemy;
            nearestDistance = distance; //proximity alone determines friendship with the turret
        }
    }

    return nearestEnemy;
}

void CreateSpike(Cell cell,std::vector<Spikes>& spikes) {
    Rectangle cellOfSpike = {cell.col*TILE_SIZE,cell.row*TILE_SIZE,TILE_SIZE,TILE_SIZE};
    Spikes newSpike;
    newSpike.cell = cell;
    newSpike.rec = cellOfSpike;
   
   spikes.push_back(newSpike);
}
void UpdateSpikes(std::vector<Enemy>& enemies, std::vector<Spikes>& spikes, int tiles[TILE_COUNT][TILE_COUNT]){
    for (Spikes& spike : spikes) {
        for (Enemy& enemy : enemies) {
            if (CheckCollisionCircleRec(enemy.position, ENEMY_RADIUS, spike.rec)) {
                //deal damage
                enemy.health = enemy.health - spike.damage;
                spike.health = 0;
                tiles[spike.cell.row][spike.cell.col] = DIRT;

                //delete spike

            }

        }
    }
    spikes.erase(
        std::remove_if(spikes.begin(), spikes.end(), [](const Spikes& spike) {

            return (spike.health == 0);
            }),
        spikes.end());
    
}

