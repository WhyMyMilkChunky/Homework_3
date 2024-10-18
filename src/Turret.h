#pragma once
#include "raylib.h"
#include <vector>
#include "enemies.h"

struct Turret {
    Vector2 pos;
    float range;
    int damage;
    float firingRate;
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

void DrawTurrets(const std::vector<Turret>& turrets, Texture2D turretTexture);

void UpdateTurrets(std::vector<Turret>& turrets, std::vector<Bullet>& bullets, const std::vector<Enemy>& enemies, float dt);

Enemy* FindNearestEnemy(const Turret& turret, const std::vector<Enemy>& enemies);

