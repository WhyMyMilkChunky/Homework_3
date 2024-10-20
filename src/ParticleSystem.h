#ifndef PARTICLE_H
#define PARTICLE_H

#include "raylib.h"
#include <vector>


//there is definatley a better way to do this but i just wanted it working


//properties of the partcles
struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float lifeTime;
    float size;
    float opacity;
};

class ParticleSystem {
public:
    ParticleSystem();
    void CreateExplosion(Vector2 position, int particleCount, float lifetime, float size, Color color, float opacity, float velocity);
    void CreateSmoke(Vector2 position, int particleCount, float lifetime, float size, Color color, float opacity, float velocity);
    void CreateMuzzleFlash(Vector2 position, Vector2 direction, int particleCount, float lifetime, float size, Color color, float opacity, float speed, float spread);
    void CreateSnow(int particleCount, float lifetime, float size, float speed, float spread, Color color, float opacity, int width, int height);
    void Update(float deltaTime);
    void Clear();
    void Draw();  

private:
    std::vector<Particle> particles;
};

#endif
