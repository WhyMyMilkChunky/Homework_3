#include "enemies.h"
#include "raylib.h"
#include "Math.h"
#include <vector>
#include "tiles.h"

extern int currentHealth;
extern int playerPoints;

Enemy CreateEnemy(EnemyType type, Vector2 startPosition) {
    // Oh look, another function to create enemies. Exciting, right?
    Enemy enemy;
    enemy.position = startPosition;
    enemy.currentWaypoint = 0;
    enemy.type = type;

    // let's give each enemy type some arbitrary stats. No favoritism here, I swear.
    switch (type) {
    case EnemyType::BASIC:
        enemy.health = enemy.maxHealth = 100;
        enemy.speed = 100;
        enemy.pointWorth = 10;
        break;
    case EnemyType::WALLACE:
        enemy.health = enemy.maxHealth = 50;
        enemy.speed = 150;
        enemy.pointWorth = 1;
        break;
    case EnemyType::JOSS:
        enemy.health = enemy.maxHealth = 300;
        enemy.speed = 50.0f;
        enemy.pointWorth = 50;
        break;
    case EnemyType::RUI:
        enemy.health = enemy.maxHealth = 20;
        enemy.speed = 200;
        enemy.pointWorth = 5;
        break;
    case EnemyType::CONNOR:
        enemy.health = enemy.maxHealth = 500;
        enemy.speed = 75;
        enemy.pointWorth = 69;
        break;
    }

    //congrats, enemy is born
    return enemy;
}

void UpdateEnemies(std::vector<Enemy>& enemies, const std::vector<Cell>& waypoints, float dt, std::vector<FloatingText>& floatingTexts) {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
            int curr = enemy.currentWaypoint;

            if (curr >= waypoints.size() - 1) {
                currentHealth -= DAMAGE_PER_ENEMY;
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
            if (enemy.health <= 0) {
                playerPoints += enemy.pointWorth;

                // Add floating text at the enemy's position
                FloatingText newText;
                newText.pos = enemy.position;
                newText.text = "+" + std::to_string(enemy.pointWorth);
                newText.alpha = 1.0f;  // Fully visible
                newText.lifetime = 1.0f;  // Lasts for 1 second
                floatingTexts.push_back(newText);

                return true;
            }

            return false;
            }),
        enemies.end()
    );
}

void DrawEnemies(const std::vector<Enemy>& enemies, Texture2D enemyTexture) {
    for (const Enemy& enemy : enemies) {
        Color tint;

        //no favorites here, nuh uh, nope, not at all
        switch (enemy.type) {
        case EnemyType::BASIC:
            tint = WHITE;
            break;
        case EnemyType::WALLACE:
            tint = BLUE;
            break;
        case EnemyType::JOSS:
            tint = GREEN;
            break;
        case EnemyType::RUI:
            tint = YELLOW;
            break;
        case EnemyType::CONNOR://this must be a really cool EnemyType
            tint = RED;
            break;
        }
        DrawTexturePro(enemyTexture,
            { 0, 0, (float)enemyTexture.width, (float)enemyTexture.height },

            { enemy.position.x, enemy.position.y, ENEMY_RADIUS * 2, ENEMY_RADIUS * 2 },
            { ENEMY_RADIUS, ENEMY_RADIUS },
            0,
            tint);

        DrawEnemyHealthBar(enemy.position, enemy.health, enemy.maxHealth, ENEMY_RADIUS);
    }
}














//UI

void DrawEnemyHealthBar(Vector2 enemyPosition, int currentHealth, int maxHealth, float enemyRadius) {
    //makehealth bars proportional
    const int BASE_HEALTH_BAR_WIDTH = 50;
    const int HEALTH_BAR_HEIGHT = 5;
    const int HEALTH_BAR_OUTLINE_THICKNESS = 2;
    const int MIN_HEALTH_BAR_WIDTH = 30;
    const int MAX_HEALTH_BAR_WIDTH = 150;

    //bar size
    float healthPercentage = (float)currentHealth / (float)maxHealth;
    int healthBarWidth = BASE_HEALTH_BAR_WIDTH + (maxHealth / 50);

    if (healthBarWidth < MIN_HEALTH_BAR_WIDTH)
        healthBarWidth = MIN_HEALTH_BAR_WIDTH;
    else if (healthBarWidth > MAX_HEALTH_BAR_WIDTH)
        healthBarWidth = MAX_HEALTH_BAR_WIDTH;

    //inner health bar
    int innerBarWidth = (int)(healthBarWidth * healthPercentage);
    int posX = (int)(enemyPosition.x - healthBarWidth / 2);
    int posY = (int)(enemyPosition.y - enemyRadius - 10);

    // is the enemy almost dead? we reflect that visually, because we’re nice like that
    Color healthColor;
    if (healthPercentage > 0.7f)
        healthColor = GREEN;
    else if (healthPercentage > 0.3f)
        healthColor = YELLOW;
    else
        healthColor = RED;

    //outlines look better on UI
    Rectangle outerRect = { (float)posX - HEALTH_BAR_OUTLINE_THICKNESS, (float)posY - HEALTH_BAR_OUTLINE_THICKNESS,
                            (float)healthBarWidth + 2 * HEALTH_BAR_OUTLINE_THICKNESS,
                            (float)HEALTH_BAR_HEIGHT + 2 * HEALTH_BAR_OUTLINE_THICKNESS };
    DrawRectangleRec(outerRect, BLACK);

    //the part that shows you how much pain they’re in
    Rectangle innerRect = { (float)posX, (float)posY, (float)innerBarWidth, (float)HEALTH_BAR_HEIGHT };
    DrawRectangleRec(innerRect, healthColor);
}
void UpdateFloatingTexts(std::vector<FloatingText>& floatingTexts, float dt) {
    for (auto& text : floatingTexts) {
        text.pos.y -= 50.0f * dt;  // Move text upward
        text.alpha -= 0.5f * dt;  // Reduce transparency
        text.lifetime -= dt;  // Reduce lifeTime
    }
    // Remove text that's fully faded or expired
    floatingTexts.erase(
        std::remove_if(floatingTexts.begin(), floatingTexts.end(),
            [](const FloatingText& text) { return text.alpha <= 0.0f || text.lifetime <= 0.0f; }),
        floatingTexts.end());
}

void DrawFloatingTexts(const std::vector<FloatingText>& floatingTexts) {
    for (const auto& text : floatingTexts) {
        Color textColor = Fade(WHITE, text.alpha);
        DrawText(text.text.c_str(), (int)text.pos.x, (int)text.pos.y, 20, textColor);
    }
}
