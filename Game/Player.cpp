#include "Player.h"
#include "Room.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include "openglframework.h"
#include "gamedefs.h"
#include "openGLStuff.h"
#include "iostream"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


Player::Player(float x, float y, float speed, float jumpHeight, float gravity, float tileSize)
    : x(x), y(y), width(tileSize), height(2 * tileSize * .8f), speed(speed),
    jumpHeight(jumpHeight), gravity(gravity), tileSize(tileSize), health(30), isInMorphBallState(false),
    timeSinceLastMorphballToggle(morphBallToggleCooldown), iframeDuration(2), iframeTimer(0), maxHealth(99), missileAmmo(0), timeSinceLastMissileToggle(0)
{
    originalWidth = width;
    originalHeight = 2 * tileSize; // Store the original height (100%)
}



void Player::update(float deltaTime, const Room& currentRoom)
{

    // If the player is not on the ground and not jumping, they have walked off a ledge
    if (!isOnGround(currentRoom) && !isJumping) {
        hasWalkedOffLedge = true;
    }
    applyGravity(deltaTime, currentRoom);

    // Update invincibility timer if invincible
    if (isInvincible) {
        iframeTimer += deltaTime;
        if (iframeTimer >= iframeDuration) {
            isInvincible = false;
            iframeTimer = 0.0f; // Reset the timer
        }
    }
    // Auto switch back to power beam if out of missiles and currently using missiles
    if (isUsingMissiles() && getMissileAmmo() <= 0) {
        usingMissiles = false;
    }

    if (timeSinceLastMissileToggle < missileToggleCooldown) {
        timeSinceLastMissileToggle += deltaTime;
    }
    if (timeSinceLastMorphballToggle < morphBallToggleCooldown) {
        timeSinceLastMorphballToggle += deltaTime;
    }
}


void Player::render() {

    // Set player color to orange (RGB: 1.0f, 0.5f, 0.0f)
    glColor3f(1.0f, 0.5f, 0.0f);

    if (isInMorphBallState) {
        const int numSegments = 20;  // Number of segments for smoother circle
        const float radius = width / 2; // Assuming width is the diameter

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); // Center of circle
        for (int i = 0; i <= numSegments; i++) {
            float theta = 2.0f * M_PI * float(i) / float(numSegments);
            float cx = radius * cosf(theta);
            float cy = radius * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    }
    else {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glBegin(GL_QUADS);

        // Draw the player as a rectangle
        glVertex2f(x - width / 2, y - height / 2); // Bottom Left
        glVertex2f(x + width / 2, y - height / 2); // Bottom Right
        glVertex2f(x + width / 2, y + height / 2); // Top Right
        glVertex2f(x - width / 2, y + height / 2); // Top Left

        glEnd();
    }
}


void Player::moveLeft(float deltaTime, const Room& currentRoom) {
    float newX = x - speed * deltaTime;
    if (canMoveTo(newX, y, currentRoom)) {
        x = newX;
    }
    facingDirection = -1; // Facing left
}

void Player::moveRight(float deltaTime, const Room& currentRoom) {
    float newX = x + speed * deltaTime;
    if (canMoveTo(newX, y, currentRoom)) {
        x = newX;
    }
    facingDirection = 1; // Facing right
}

void Player::jump(float deltaTime, const Room& currentRoom) {
    if (isOnGround(currentRoom) && !isJumping) {
        // Initiate jump only if there's no block directly above
        int gridX = static_cast<int>(x / currentRoom.getTileSize());
        int gridYAbove = static_cast<int>((y + height / 2) / currentRoom.getTileSize());

        if (!currentRoom.isCellBlocked(gridX, gridYAbove)) {
            isJumping = true;
            verticalSpeed = jumpHeight;
        }
    }
}

bool Player::isOnGround(const Room& currentRoom) const {
    int gridX = static_cast<int>(x / currentRoom.getTileSize());
    int gridY = static_cast<int>((y - height / 2) / currentRoom.getTileSize()) - 1;

    return (y == 0) || currentRoom.isCellBlocked(gridX, gridY);
}

void Player::applyGravity(float deltaTime, const Room& currentRoom) {
    if (!isOnGround(currentRoom) || isJumping || hasWalkedOffLedge) {
        verticalSpeed -= gravity * deltaTime;

        // Terminal velocity check
        const float terminalVelocity = -500.0f; // Adjust as needed
        if (verticalSpeed < terminalVelocity) {
            verticalSpeed = terminalVelocity;
        }

        float potentialY = y + verticalSpeed * deltaTime;

        // Debugging

        int gridX = static_cast<int>(x / currentRoom.getTileSize());
        int gridYBelow = static_cast<int>((potentialY - height / 2) / currentRoom.getTileSize());

        // Falling logic
        if (verticalSpeed <= 0) {
            if (currentRoom.isCellBlocked(gridX, gridYBelow)) {
                y = (gridYBelow + 1) * currentRoom.getTileSize() + height / 2;
                verticalSpeed = 0;
                isJumping = false;
                hasWalkedOffLedge = false;
            }
            else {
                y = potentialY;
            }
        }
        else { // Jumping logic
            int gridYAbove = static_cast<int>((potentialY + height / 2) / currentRoom.getTileSize());
            if (currentRoom.isCellBlocked(gridX, gridYAbove)) {
                y = gridYAbove * currentRoom.getTileSize() - height / 2;
                verticalSpeed = 0;
                isJumping = false;
            }
            else {
                y = potentialY;
            }
        }
    }
}
bool Player::canMoveTo(float newX, float newY, const Room& currentRoom) const {
    // Checking for collision at the bottom of the player
    int gridXBottom = static_cast<int>(newX / currentRoom.getTileSize());
    int gridYBottom = static_cast<int>((newY - height / 2) / currentRoom.getTileSize());

    if (currentRoom.isCellBlocked(gridXBottom, gridYBottom)) {
        return false; // Collision detected at the bottom
    }

    // Checking for collision at the top of the player
    int gridXTop = static_cast<int>(newX / currentRoom.getTileSize());
    int gridYTop = static_cast<int>((newY + height / 2 - 1) / currentRoom.getTileSize());

    if (currentRoom.isCellBlocked(gridXTop, gridYTop)) {
        return false; // Collision detected at the top
    }

    return true; // No collision detected
}

float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

int Player::getHealth() const {
    return health;
}

void Player::updateIframes(float deltaTime) {
    if (isInvincible) {
        iframeTimer += deltaTime;
        if (iframeTimer >= iframeDuration) {
            isInvincible = false;
            iframeTimer = 0.0f; // Reset the timer

        }
    }

}

void Player::decreaseHealth(int amount) {
    if (!isInvincible) {
        health -= amount;
        if (health < 0) health = 0;
        if (health <= 0) {
            PostQuitMessage(0);  // Signal the application to close
        }
        else {
            isInvincible = true;  // Activate invincibility
            iframeTimer = .0f;  // Reset the timer

        }
    }
}

std::pair<float, float> Player::getSize() const {
    return { width, height };
}

int Player::getFacingDirection() const {
    return facingDirection;
}
void Player::toggleMissiles() {
    if (hasAcquiredMissiles && timeSinceLastMissileToggle >= missileToggleCooldown) {
        usingMissiles = !usingMissiles;
        timeSinceLastMissileToggle = 0.0f;  // Reset cooldown timer

        // Debug message for successful toggle
        std::cout << "Missiles toggled. New state: " << (usingMissiles ? "On" : "Off") << std::endl;
    }
    else {
        // Debug messages for conditions that prevent missile toggling
        if (!hasAcquiredMissiles) {
            std::cout << "Missiles not acquired, cannot toggle." << std::endl;
        }
        else if (timeSinceLastMissileToggle < missileToggleCooldown) {
            std::cout << "Missile toggle on cooldown. Time remaining: "
                << (missileToggleCooldown - timeSinceLastMissileToggle) << " seconds." << std::endl;
        }
    }
}



// Check if the player is currently using missiles
bool Player::isUsingMissiles() const {
    return usingMissiles;
}

// Check if the player can fire a missile (e.g., has ammo)
bool Player::canFireMissile() const {
    return missileAmmo > 0;
}

// Consume a missile upon firing
void Player::consumeMissile() {
    if (missileAmmo > 0) {
        missileAmmo--;
    }
}
int Player::getMissileAmmo() const {
    return missileAmmo;
}
void Player::increaseHealth(int amount) 
{
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}
void Player::increaseMissileAmmo(int amount) {
    missileAmmo += amount;
}
void Player::toggleMorphBall() {
    if (hasAcquiredMorphBall && timeSinceLastMorphballToggle >= morphBallToggleCooldown) {
        isInMorphBallState = !isInMorphBallState;

        if (isInMorphBallState) {
            // If we are now in Morph Ball form, reduce the height and set the flags
            height = tileSize;
            verticalSpeed = -1.0f; // Start falling
            isJumping = false;
            hasWalkedOffLedge = true;
        }
        else {
            // If we are reverting to normal form, increase the height
            // Adjust the Y position upwards so that the player does not get stuck in the floor
            float deltaY = (originalHeight - height) / 2;
            y += deltaY; // Move the player up by half the difference in height
            height = originalHeight * 0.8f; // Set height to 90% of original height
        }

        // Reset the cooldown timer
        timeSinceLastMorphballToggle = 0.0f;
    }
}

bool Player::isInMorphBallMode() const {
    return isInMorphBallState;
}


bool Player::canToggleMorphBall() const {
    return timeSinceLastMorphballToggle >= morphBallToggleCooldown;
}

void Player::updateCooldowns(float deltaTime) {
    timeSinceLastMorphballToggle += deltaTime;
}
void Player::acquireMorphBall() {
    hasAcquiredMorphBall = true;
}

void Player::acquireMissiles() {
    hasAcquiredMissiles = true;
}
