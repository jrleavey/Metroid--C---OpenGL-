#include "Skree.h"
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include "openglframework.h"  
#include "gamedefs.h"
#include "openGLStuff.h"
#include "SOIL.h"

Skree::Skree(float x, float y, float size, float speed, Room& room)
    : Enemy(), x(x), y(y), size(size), speed(speed), room(room), player(nullptr), dead(false), onGround(false), explosionTimer(0.0f), frameDuration(.3f),currentFrame(0),animationTimer(0.0f)
{

    health = 10;
    SkreeTexture = SOIL_load_OGL_texture("asset/Skree Sprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

}
static const int totalFrames = 3; // Total number of frames in the animation

void Skree::update(float deltaTime) {
    if (dead) {
        explosionTimer += deltaTime;
        if (explosionTimer >= EXPLOSION_DELAY) {
            // Explosion logic (not detailed here)
        }
        return;
    }

    // Check if the player is present
    if (player != nullptr) {
        float playerDistanceX = std::abs(player->getX() - x);
        float playerDistanceY = player->getY() - y;
        const float activationDistance = room.getTileSize() * 3; // Activate within 3 tiles

        // Check if the player is close enough on the X-axis and below Skree
        if (playerDistanceX <= activationDistance && playerDistanceY < 0 && !diving) {
            diving = true; // Start diving if player is below and within range
        }
    }

    // If Skree is diving
    if (diving) {
        y -= speed * deltaTime; // Dive downwards

        // Adjust Skree's X position slightly towards the player
        if (player != nullptr) {
            float direction = player->getX() > x ? 1 : -1; // Determine direction towards player
            x += direction * (speed / 2) * deltaTime; // Half of vertical speed for horizontal movement
        }

        // Check for collision with ground or blocked tile
        if (room.isCellBlocked(static_cast<int>(x / room.getTileSize()), static_cast<int>(y / room.getTileSize()))) {
            dead = true; // Skree stops moving and will explode
        }
    }

    // Animation update
    float animationSpeed = diving ? frameDuration / 2 : frameDuration; // Faster animation speed when diving
    animationTimer += deltaTime;
    if (animationTimer >= animationSpeed) {
        currentFrame = (currentFrame + 1) % totalFrames; // Cycle through frames
        animationTimer -= animationSpeed;
    }
}



void Skree::render() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, SkreeTexture);

    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white to use the texture's original colors

    float texLeft, texRight;
    switch (currentFrame) {
    case 0: // First frame
        texLeft = 0.0f;
        texRight = 1.0f / 3.0f; // One third of the texture for the first frame
        break;
    case 1: // Second frame
        texLeft = 1.0f / 3.0f;
        texRight = 2.0f / 3.0f; // Second third for the second frame
        break;
    case 2: // Third frame
        texLeft = 2.0f / 3.0f;
        texRight = 1.0f; // Last third for the third frame
        break;
    }

    // Texture coordinates for vertical flip
    float texTop = 1.0f;    // Top edge of the texture (flipped)
    float texBottom = 0.0f; // Bottom edge of the texture (flipped)

    float renderHeight = size * 2; // Assuming 'size' is the height of one tile

    float left = x - size / 2;
    float right = x + size / 2;
    float bottom = y - renderHeight / 2;
    float top = y + renderHeight / 2;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(left, bottom);
    glTexCoord2f(texRight, texTop);
    glVertex2f(right, bottom);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(right, top);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(left, top);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}





void Skree::decreaseHealth(int damage) {
    health -= damage;
    gSoundManager.PlaySoundW(ENEMY_DAMAGE);
    if (health <= 0) {
        dead = true;
    }
}

bool Skree::isDead() const {
    return dead;
}

// explode method removed

bool Skree::isAbovePlayer() const {
    if (!player) return false;
    // Check if Skree's X position is near the player's X position
    // ...
    return false;
}

float Skree::getX() const {
    return x;
}

float Skree::getY() const {
    return y;
}

void Skree::setPlayer(const Player& player) {
    this->player = &player;
}
float Skree::getSize() const {
    return size;
}