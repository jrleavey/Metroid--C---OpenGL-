#define NOMINMAX
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
#include <algorithm>
#include "SOIL.h"
#include "SoundManager.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



Player::Player(float x, float y, float speed, float jumpHeight, float gravity, float tileSize)
    : x(x), y(y), width(tileSize), height(2 * tileSize * 0.9f), speed(speed),
    jumpHeight(jumpHeight), gravity(gravity), tileSize(tileSize), health(30),
    isInMorphBallState(false), timeSinceLastMorphballToggle(morphBallToggleCooldown),
    iframeDuration(2), iframeTimer(0), maxHealth(99), missileAmmo(0),
    timeSinceLastMissileToggle(0), morphballFrame(1), morphballAnimationTimer(0.0f),
    isFacingUp(false), isMoving(false)
{
    originalWidth = width;
    originalHeight = 2 * tileSize; // Store the original height (100%)

    // Load the texture for Morphball sprites
    morphballTexture = SOIL_load_OGL_texture("asset/Morphball.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    playerTexture = SOIL_load_OGL_texture("asset/Samus Sprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    currentFrame = 0;
    animationTimer = 0.0f;

    for (int i = 0; i < 4; ++i) 
    {
        energyTankFull[i] = false; // Initialize all energy tanks as empty
    }
    numEnergyTanks = 0; // Start with no Energy Tanks
}



void Player::update(float deltaTime, const Room& currentRoom) {
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
    // Update animation frame
    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        if (isMoving && !isJumping && !isFacingUp) {
            // Moving, not jumping, not looking up: Cycle Sprites 2-4
            currentFrame = (currentFrame >= 2 && currentFrame < 4) ? currentFrame + 1 : 2;

            // Footsteps sound logic
            timeSinceLastFootstep += deltaTime;
            if (timeSinceLastFootstep >= FOOTSTEP_INTERVAL && isOnGround(currentRoom)) {
                gSoundManager.PlaySound(SAMUS_FOOTSTEP); // Replace with actual sound ID
                timeSinceLastFootstep = 0.0f;
            }
        }
        else if (isJumping && !isFacingUp) {
            // Jumping but not looking up: Use Sprite 5
            currentFrame = 4;
        }
        else if (isFacingUp && !isMoving && !isJumping) {
            // Looking up, not moving, not jumping: Use Sprite 6
            currentFrame = 5;
        }
        else if (isMoving && isFacingUp && !isJumping) {
            // Moving and looking up, but not jumping: Cycle Sprites 7-9
            currentFrame = (currentFrame >= 7 && currentFrame < 9) ? currentFrame + 1 : 7;
        }
        else if (isJumping && isFacingUp) {
            // Moving, jumping, and looking up: Use Sprite 10
            currentFrame = 9;
        }
        else {
            // Idle (not moving, not looking up, not jumping): Use Sprite 1
            currentFrame = 0;
        }
        animationTimer = 0.0f;
    }
    else if (isMoving && isOnGround(currentRoom)) {
        // Footsteps sound logic when not changing frame
        timeSinceLastFootstep += deltaTime;
        if (timeSinceLastFootstep >= FOOTSTEP_INTERVAL) {
            gSoundManager.PlaySound(SAMUS_FOOTSTEP); // Replace with actual sound ID
            timeSinceLastFootstep = 0.0f;
        }
    }

    // Resetting isMoving to false to be updated by moveLeft/moveRight functions in the next frame
    isMoving = false;
    // Handle morphball animation
    if (isInMorphBallState) {
        morphballAnimationTimer += deltaTime;
        if (morphballAnimationTimer >= morphballFrameDuration) {
            // Only animate if moving
            if (std::abs(x - lastX) > 0.01f) {
                morphballFrame = (morphballFrame + 1) % 4; // There are 4 frames
            }
            morphballAnimationTimer = 0.0f;
        }
    }

    lastX = x; // Store the last x position for movement detection
}



void Player::render() {
    if (isInMorphBallState) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, morphballTexture);

        // Calculate texture coordinates for the current morphball frame
        float frameWidth = 1.0f / 4.0f; // 4 frames in the texture
        float texLeft = morphballFrame * frameWidth;
        float texRight = texLeft + frameWidth;
        float texTop = 1.0f;
        float texBottom = 0.0f;

        // Shrink the visual representation by 20%
        float scale = 0.8f; // 20% smaller
        float radius = (width / 2) * scale; // Applying scale to radius
        float left = x - radius;
        float right = x + radius;
        float bottom = y - radius;
        float top = y + radius;

        glColor3f(1.0f, 1.0f, 1.0f); // White color for the texture
        glBegin(GL_QUADS);
        glTexCoord2f(texLeft, texBottom); glVertex2f(left, bottom);
        glTexCoord2f(texRight, texBottom); glVertex2f(right, bottom);
        glTexCoord2f(texRight, texTop); glVertex2f(right, top);
        glTexCoord2f(texLeft, texTop); glVertex2f(left, top);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, playerTexture);

        // Calculate texture coordinates for the current frame
        float frameWidth = 1.0f / 10.0f; // Assuming 10 frames in the texture
        float texLeft = currentFrame * frameWidth;
        float texRight = texLeft + frameWidth;

        // Flip texture coordinates if facing left
        if (facingDirection == -1) {
            std::swap(texLeft, texRight);
        }

        // Increase the visual representation by 20%
        float scale = 1.2f; // 20% larger
        float halfWidth = (width / 2) * scale;
        float halfHeight = (height / 2) * scale;

        glColor3f(1.0f, 1.0f, 1.0f); // White color for the texture
        glBegin(GL_QUADS);
        glTexCoord2f(texLeft, 1.0f); glVertex2f(x - halfWidth, y - halfHeight);
        glTexCoord2f(texRight, 1.0f); glVertex2f(x + halfWidth, y - halfHeight);
        glTexCoord2f(texRight, 0.0f); glVertex2f(x + halfWidth, y + halfHeight);
        glTexCoord2f(texLeft, 0.0f); glVertex2f(x - halfWidth, y + halfHeight);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}


void Player::moveLeft(float deltaTime, const Room& currentRoom) {
    float newX = x - speed * deltaTime;
    if (canMoveTo(newX, y, currentRoom)) {
        x = newX;
    }
    facingDirection = -1; // Facing left
    isMoving = true;
}

void Player::moveRight(float deltaTime, const Room& currentRoom) {
    float newX = x + speed * deltaTime;
    if (canMoveTo(newX, y, currentRoom)) {
        x = newX;
    }
    facingDirection = 1; // Facing right
    isMoving = true;

}

void Player::jump(float deltaTime, const Room& currentRoom) {
    if (isOnGround(currentRoom) && !isJumping) {
        // Initiate jump only if there's no block directly above
        int gridX = static_cast<int>(x / currentRoom.getTileSize());
        int gridYAbove = static_cast<int>((y + height / 2) / currentRoom.getTileSize());

        if (!currentRoom.isCellBlocked(gridX, gridYAbove)) {
            isJumping = true;
            verticalSpeed = jumpHeight;
            // Example of playing the Jump sound
            gSoundManager.PlaySoundW(JUMP);
        }
    }
}
void Player::airborneJump(float deltaTime, const Room& currentRoom) {
    // Check if there's no block directly above the player
    int gridX = static_cast<int>(x / currentRoom.getTileSize());
    int gridYAbove = static_cast<int>((y + height / 2) / currentRoom.getTileSize());

    if (!currentRoom.isCellBlocked(gridX, gridYAbove)) {
        isJumping = true;
        verticalSpeed = jumpHeight; // Reset vertical speed to the initial jump height
    }
}



bool Player::isOnGround(const Room& currentRoom) const {
    float gridX = x / currentRoom.getTileSize();
    float gridY = (y - height / 2) / currentRoom.getTileSize() - 1;

    return currentRoom.isCellBlocked(std::floor(gridX), std::floor(gridY));
}



void Player::applyGravity(float deltaTime, const Room& currentRoom) {
    if (!isOnGround(currentRoom) || isJumping || hasWalkedOffLedge) {
        verticalSpeed -= gravity * deltaTime;
        const float terminalVelocity = -500.0f;
        if (verticalSpeed < terminalVelocity) {
            verticalSpeed = terminalVelocity;
        }

        float potentialY = y + verticalSpeed * deltaTime;

        float gridX = x / currentRoom.getTileSize();
        float gridYBelow = (potentialY - height / 2) / currentRoom.getTileSize();

        if (verticalSpeed <= 0) {
            if (currentRoom.isCellBlocked(std::floor(gridX), std::floor(gridYBelow))) {
                y = (std::floor(gridYBelow) + 1) * currentRoom.getTileSize() + height / 2;
                verticalSpeed = 0;
                isJumping = false;
                hasWalkedOffLedge = false;
            }
            else {
                y = potentialY;
            }
        }
        else {
            float gridYAbove = (potentialY + height / 2) / currentRoom.getTileSize();
            if (currentRoom.isCellBlocked(std::floor(gridX), std::floor(gridYAbove))) {
                y = std::floor(gridYAbove) * currentRoom.getTileSize() - height / 2;
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

void Player::increaseHealth(int amount) {
    health += amount;
    if (health > 99) {
        int excess = health - 99;
        health = 99; // Set health to 99

        // Fill an empty Energy Tank with the excess health
        for (int i = 0; i < numEnergyTanks; i++) {
            if (!energyTankFull[i]) {
                energyTankFull[i] = true; // Mark the tank as full
                excess -= 99; // Reduce the excess health by the capacity of one tank
                if (excess <= 0) {
                    break;
                }
            }
        }
    }
}

void Player::decreaseHealth(int amount) {
    if (!isInvincible) {
        health -= amount;
        gSoundManager.PlaySound(TAKE_DAMAGE); // Replace with actual sound ID

        // If health drops below 0
        if (health < 0) {
            bool foundFullTank = false;

            for (int i = 0; i < numEnergyTanks; ++i) {
                if (energyTankFull[i]) {
                    // Found a full energy tank, use it
                    energyTankFull[i] = false; // Mark the tank as empty
                    int overflowDamage = -health; // Calculate remaining damage after health hits 0
                    health = 99 - overflowDamage; // Apply overflow damage to the new health
                    foundFullTank = true;
                    break; // Stop after using one tank
                }
            }

            // If no full energy tank is found and health is still 0 or below
            if (!foundFullTank && health <= 0) {
                TerminateApplication(g_window);
                return;
            }
        }

        // Other actions when player is still alive
        isInvincible = true; // Activate invincibility
        iframeTimer = 0.0f; // Reset the timer
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


void Player::increaseMissileAmmo(int amount) {
    missileAmmo += amount;
}
void Player::toggleMorphBall() {
    std::cout << "InsideToggleMorphBall" << std::endl;

    if (hasAcquiredMorphBall && timeSinceLastMorphballToggle >= morphBallToggleCooldown) {
        isInMorphBallState = !isInMorphBallState;

        if (isInMorphBallState) {
            std::cout << "isinmorphballstate" << std::endl;

            // If we are now in Morph Ball form, reduce the height and set the flags
            height = tileSize *.8f;
            verticalSpeed = -1.0f; // Start falling
            isJumping = false;
            hasWalkedOffLedge = true;
            gSoundManager.PlaySoundW(SAMUS_MORPH);

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
void Player::acquireLongBeam() {
    hasAcquiredLongBeam = true;
}
void Player::acquireEnergyTank() {
    if (numEnergyTanks < 4) {
        energyTankFull[numEnergyTanks] = true; // Mark the newly acquired tank as full
        numEnergyTanks++; // Increase the count of Energy Tanks
    }
}
void Player::acquireMorphBallBombs() {
    hasAcquiredMorphBallBombs = true;
    // Additional logic if needed
}
int Player::getEnergyTanks() const {
    return numEnergyTanks;
}

int Player::getMaxHealth() const {
    return maxHealth;
}
void Player::setPosition(float x, float y) {
    this->x = x; // Update the x position of the player
    this->y = y; // Update the y position of the player
}
void Player::setIsMoving(bool value) {
    isMoving = value;
}

void Player::setIsFacingUp(bool value) {
    isFacingUp = value;
}

bool Player::getIsMoving() const {
    return isMoving;
}

bool Player::getIsFacingUp() const {
    return isFacingUp;
}