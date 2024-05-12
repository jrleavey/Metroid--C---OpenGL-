#include "Powerup.h"

// Common constructor for all Powerup types
Powerup::Powerup(float x, float y, float size) : position({ x, y }), tileSize(size) {
    // Load the texture and initialize animation properties
    powerupTexture = SOIL_load_OGL_texture("asset/Powerup Sprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    currentFrame = 0;
    animationTimer = 0.0f;
    frameDuration = 0.25f; // Animation speed
}

// Update function common to all powerups for handling animations
void Powerup::update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        currentFrame = (currentFrame + 1) % 4; // Assuming 4 frames per powerup
        animationTimer -= frameDuration;
    }
}

// Getter for the powerup's position
Position Powerup::getPosition() const {
    return position;
}

// Common rendering logic for all powerups
void Powerup::render(int frameStart) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, powerupTexture);

    glColor3f(1.0f, 1.0f, 1.0f); // White color to show original texture colors

    // Calculate texture coordinates based on current frame
    float frameWidth = 1.0f / 20.0f; // Assuming 20 frames horizontally in the sprite sheet
    float texLeft = (frameStart + currentFrame) * frameWidth;
    float texRight = texLeft + frameWidth;

    // Flipped texture coordinates for vertical flip
    float texTop = 1.0f;    // Top edge of the texture (flipped)
    float texBottom = 0.0f; // Bottom edge of the texture (flipped)

    float halfTileSize = tileSize / 2;
    float left = position.x - halfTileSize;
    float right = position.x + halfTileSize;
    float bottom = position.y - halfTileSize;
    float top = position.y + halfTileSize;

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

MorphBallPowerup::MorphBallPowerup(float x, float y, float size) : Powerup(x, y, size) {
    // Constructor implementation (if needed)
}

void MorphBallPowerup::applyEffect(Player& player) {
    player.acquireMorphBall();
    // Additional logic if needed
}

void MorphBallPowerup::render() {
    Powerup::render(0); // MorphBall frames start at 0
}
LongBeamPowerup::LongBeamPowerup(float x, float y, float size) : Powerup(x, y, size) {
    // Constructor implementation
}

void LongBeamPowerup::applyEffect(Player& player) {
    player.acquireLongBeam();
}

void LongBeamPowerup::render() {
    Powerup::render(4); // LongBeam frames start at 4
}
MorphBallBombsPowerup::MorphBallBombsPowerup(float x, float y, float size) : Powerup(x, y, size) {
    // Constructor implementation
}

void MorphBallBombsPowerup::applyEffect(Player& player) {
    player.acquireMorphBallBombs();
}

void MorphBallBombsPowerup::render() {
    Powerup::render(8); // MorphBallBombs frames start at 8
}
MissileExpansionPowerup::MissileExpansionPowerup(float x, float y, float size) : Powerup(x, y, size) {
    // Constructor implementation (if needed)
}



void MissileExpansionPowerup::applyEffect(Player& player) {
    player.acquireMissiles();
    player.increaseMissileAmmo(5);
}

void MissileExpansionPowerup::render() {
    Powerup::render(12); // Missiles frames start at 12
}
EnergyTankPowerup::EnergyTankPowerup(float x, float y, float size) : Powerup(x, y, size) {
    // Constructor implementation (if needed)
}

void EnergyTankPowerup::applyEffect(Player& player) {
    player.acquireEnergyTank();
}

void EnergyTankPowerup::render() {
    Powerup::render(16); // Energy Tanks frames start at 16
}
