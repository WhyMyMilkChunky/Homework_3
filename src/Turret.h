#pragma once
#include "raylib.h"
#include <vector>
#include "enemies.h"
#include "AudioManager.h"


struct Turret {
    Vector2 pos;
    float range;
    int damage;
    float firingRate = 0.4F;
    float cooldown;
    float rotationAngle;
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

