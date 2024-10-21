#pragma once
#include "raylib.h"
#include <vector>
#include "enemies.h"
#include "AudioManager.h"
#include "tiles.h"


struct Turret {
    Vector2 pos;
    float range;
    int damage;
    float firingRate = 0.4F;
    float cooldown;
    float rotationAngle;
};
struct Spikes {
    Cell cell;
    Rectangle rec;
    float damage = 20;
    int health = 1;
};
struct Bullet
{
    Vector2 pos{};
    Vector2 dir{};
    float time = 0.0f;
    bool enabled = true;
};


Turret CreateTurret(Vector2 position);

void DrawTurrets(std::vector<Turret>& turrets, Texture2D turretTexture);

void UpdateTurrets(std::vector<Turret>& turrets, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, float dt, AudioManager& audioManager);

Enemy* FindNearestEnemy(Turret& turret, std::vector<Enemy>& enemies);
void CreateSpike(Cell cell, std::vector<Spikes>& spikes);
void UpdateSpikes(std::vector<Enemy>& enemies, std::vector<Spikes>& spikes,int tiles[TILE_COUNT][TILE_COUNT]);

