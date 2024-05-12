#include "MorphBallBomb.h"
#include "SoundManager.h"

const float MorphBallBomb::BOMB_SIZE = 20; // Initialize static constant for bomb size

// Constructor for MorphBallBomb
MorphBallBomb::MorphBallBomb(float x, float y, float size, Player& player)
    : position(x, y), size(size), exploded(false),
    explosionRadius(size * 2), explosionTimer(1.5f), player(player)
{
    // Constructor logic
}

// Update method for MorphBallBomb
void MorphBallBomb::update(float deltaTime, const Room& currentRoom, std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Update logic for bomb, including explosion and collision detection
    if (!exploded) {
        explosionTimer -= deltaTime;
        if (explosionTimer <= 0.0f) {
            exploded = true;
            gSoundManager.PlaySoundW(BOMB_EXPLODE);

            // Check and handle collision with player
            float playerDistance = sqrt(pow(position.x - player.getX(), 2) + pow(position.y - player.getY(), 2));
            if (playerDistance < (size + player.getSize().first)) {
                player.airborneJump(deltaTime, currentRoom);
            }

            // Check and handle collision with enemies
            for (auto& enemy : enemies) {
                float enemyDistance = sqrt(pow(position.x - enemy->getX(), 2) + pow(position.y - enemy->getY(), 2));
                if (enemyDistance < (size + enemy->getSize() / 2)) {
                    enemy->decreaseHealth(10); // Damage amount for bomb
                }
            }

            markForRemoval();
        }
    }
}

// Check if the bomb has exploded
bool MorphBallBomb::isExploded() const {
    return exploded;
}

// Check if bomb collides with the player
bool MorphBallBomb::collidesWithPlayer(const Player& player) const {
    float distance = sqrt(pow(position.x - player.getX(), 2) + pow(position.y - player.getY(), 2));
    return distance < (size + player.getSize().first);
}

// Render method for MorphBallBomb
void MorphBallBomb::render() const {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for bomb
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y); // Center of circle
    const int numSegments = 20;
    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float cx = size * cosf(theta);
        float cy = size * sinf(theta);
        glVertex2f(position.x + cx, position.y + cy);
    }
    glEnd();
}

// Mark the bomb for removal
void MorphBallBomb::markForRemoval() {
    markedForRemoval = true;
}

// Check if the bomb is marked for removal
bool MorphBallBomb::isMarkedForRemoval() const {
    return markedForRemoval;
}

// Getters for bomb's position and size
float MorphBallBomb::getX() const { return position.x; }
float MorphBallBomb::getY() const { return position.y; }
float MorphBallBomb::getSize() const { return size; }
