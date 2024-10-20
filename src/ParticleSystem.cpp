#include "ParticleSystem.h"
#include "raylib.h"
#include "Math.h"

ParticleSystem::ParticleSystem() {
    
}

void ParticleSystem::CreateMuzzleFlash(Vector2 position, Vector2 direction, int particleCount, float lifetime, float size, Color color, float opacity, float speed, float spread) {
    for (int i = 0; i < particleCount; i++) {
        Particle p;
        //spread
        float angle = atan2(direction.y, direction.x);
        float randomSpread = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * spread;

        p.velocity.x = cos(angle + randomSpread) * speed;
        p.velocity.y = sin(angle + randomSpread) * speed;

        
        p.position = position;
        p.lifeTime = lifetime;
        p.size = size;
        p.color = color;
        p.opacity = opacity;

        particles.push_back(p);
    }
}

//woohoo particles hahahahahahahah
void ParticleSystem::CreateExplosion(Vector2 position, int particleCount, float lifetime, float size, Color color, float opacity, float speed) {
    for (int i = 0; i < particleCount; i++) {
        Particle p;
        float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * PI;//pick random direction

        p.velocity.x = cos(angle) * speed;
        p.velocity.y = sin(angle) * speed;

        p.position = position;
        p.lifeTime = lifetime;
        p.size = size;
        p.color = color;
        p.opacity = opacity;

        particles.push_back(p);
    }
}

void ParticleSystem::CreateSmoke(Vector2 position, int particleCount, float lifetime, float size, Color color, float opacity, float velocity) // smoke particles
{
    for (int i = 0; i < particleCount; i++) 
    {
        Particle p;

        float angle = 90;
        float spread = (GetRandomValue(-90, 0));//idk if this is really correct, i couldnt get the smoke moving the right way and just started inputting random numbers :/

        p.velocity.x = velocity * cos(angle) + spread;
        p.velocity.y = velocity * sin(angle) + spread;
        p.position = position;
        p.lifeTime = lifetime;
        p.size = size;
        p.color = color;
        p.opacity = opacity;
        particles.push_back(p);
    }

}
void ParticleSystem::CreateSnow(int particleCount, float lifetime, float size, float speed, float spread, Color color, float opacity, int screenWidth, bool prewarm) {
    for (int i = 0; i < particleCount; i++) {
        Particle p;
        p.position.x = static_cast<float>(rand() % screenWidth);

        if (prewarm) {
                       p.position.y = static_cast<float>(rand() % GetScreenHeight());  
            p.lifeTime = lifetime * (1.0f - p.position.y / static_cast<float>(GetScreenHeight()));
        }
        else {
            p.position.y = -size; 
            p.lifeTime = lifetime;
        }
        p.velocity.x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * spread;
        p.velocity.y = speed;

      
        p.size = size;
        p.color = color;
        p.opacity = opacity;

        particles.push_back(p);
    }
}
void ParticleSystem::Update(float deltaTime) {
    for (auto& p : particles) {
        p.position.x += p.velocity.x * deltaTime;
        p.position.y += p.velocity.y * deltaTime;
        p.lifeTime -= deltaTime;
        p.opacity -= deltaTime / p.lifeTime;
    }

    
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& p) {
        return p.lifeTime <= 0.0f || p.opacity <= 0.0f;
        }), particles.end());
}


void ParticleSystem::Draw() {
    for (const auto& p : particles) {
        Color colorWithOpacity = p.color;
        colorWithOpacity.a = static_cast<unsigned char>(255 * p.opacity);
        DrawCircleV(p.position, p.size, colorWithOpacity);
    }
}
