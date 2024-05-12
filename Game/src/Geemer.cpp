#include "Geemer.h"
#include "SOIL.h"

// Assuming GRAVITY is a constant value defined elsewhere
const float GRAVITY = 100.0f;

// Constructor for Geemer class
Geemer::Geemer(float x, float y, float size, float speed, Room& room)
    : Enemy(), x(x), y(y), size(size), speed(speed), room(room),
    direction(Direction::RIGHT), onGround(false), isClimbing(false),
    currentFrame(0), animationTimer(0.0f) {
    health = 20; // Initialize health
    // Load texture for Geemer
    GeemerTexture = SOIL_load_OGL_texture("asset/Geemer Sprites.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

// Update method for Geemer
void Geemer::update(float deltaTime) {
    // Gravity effect when not on ground and not climbing
    if (!onGround && !isClimbing) {
        y -= GRAVITY * deltaTime; // Apply gravity
        if (isOnGround()) {
            onGround = true;
            y += (GRAVITY * deltaTime); // Correct position if below ground
            direction = Direction::RIGHT; // Start moving right when on the ground
        }
    }

    // Movement logic when on ground or climbing
    if (onGround || isClimbing) {
        if (isAtEdge()) {
            rotate(); // Rotate if at an edge
        }
        else {
            moveAlongSurface(deltaTime); // Move along the surface
        }
    }

    // Animation logic
    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        currentFrame = (currentFrame + 1) % 2; // Cycle through frames
        animationTimer -= frameDuration; // Reset timer
    }
}

// Check if Geemer is at an edge
bool Geemer::isAtEdge() const {
    int nextX = static_cast<int>((direction == Direction::LEFT ? x - size : x + size) / room.getTileSize());
    int currentY = static_cast<int>(y / room.getTileSize());
    // Edge detection logic
    return room.isCellBlocked(nextX, currentY) || !room.isCellBlocked(nextX, currentY - 1);
}

// Rotate Geemer's direction
void Geemer::rotate() {
    // Switch direction between left and right
    direction = (direction == Direction::LEFT ? Direction::RIGHT : Direction::LEFT);
}

// Render method for Geemer
void Geemer::render() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, GeemerTexture);

    // Texture coordinates based on current frame
    float texLeft = currentFrame == 0 ? 0.0f : 0.5f;
    float texRight = currentFrame == 0 ? 0.5f : 1.0f;
    float texTop = 0.0f;
    float texBottom = 1.0f;

    // Set up vertices for rendering
    float left = x - size / 2;
    float right = x + size / 2;
    float bottom = y - size / 2;
    float top = y + size / 2;

    glBegin(GL_QUADS);
    // Assign texture coordinates and vertices
    glTexCoord2f(texLeft, texBottom); glVertex2f(left, bottom);
    glTexCoord2f(texRight, texBottom); glVertex2f(right, bottom);
    glTexCoord2f(texRight, texTop); glVertex2f(right, top);
    glTexCoord2f(texLeft, texTop); glVertex2f(left, top);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

// Check if Geemer is on the ground
bool Geemer::isOnGround() const {
    int cellX = static_cast<int>(x / room.getTileSize());
    int cellY = static_cast<int>((y - size / 2) / room.getTileSize());
    // Ground detection logic
    return room.isCellBlocked(cellX, cellY);
}

// Check if the next cell in movement direction is blocked
bool Geemer::isNextCellBlocked() const {
    int nextX = x / room.getTileSize();
    int nextY = y / room.getTileSize();
    adjustDirectionForMovement(nextX, nextY); // Adjust direction
    return room.isCellBlocked(nextX, nextY);
}

// Check if the next tile in movement direction is empty
bool Geemer::isNextTileEmpty() const {
    int nextX = x / room.getTileSize();
    int nextY = y / room.getTileSize();
    adjustDirectionForMovement(nextX, nextY); // Adjust direction
    return !room.isCellBlocked(nextX, nextY);
}

// Move Geemer along a surface
void Geemer::moveAlongSurface(float deltaTime) {
    float deltaMove = speed * deltaTime;
    // Movement logic based on direction
    switch (direction) {
    case Direction::LEFT: x -= deltaMove; break;
    case Direction::RIGHT: x += deltaMove; break;
    case Direction::UP:
    case Direction::DOWN:
        // Currently, vertical movement is disabled
        break;
    }
}

// Adjust the next X and Y coordinates based on direction
void Geemer::adjustDirectionForMovement(int& nextX, int& nextY) const {
    // Direction adjustment logic
    switch (direction) {
    case Direction::LEFT: nextX--; break;
    case Direction::RIGHT: nextX++; break;
    case Direction::UP: nextY++; break;
    case Direction::DOWN: nextY--; break;
    }
}

// Getters for position and size
float Geemer::getX() const { return x; }
float Geemer::getY() const { return y; }
float Geemer::getSize() const { return size; }

// Decrease health when taking damage
void Geemer::decreaseHealth(int damage) {
    health -= damage; // Reduce health
    gSoundManager.PlaySoundW(ENEMY_DAMAGE); // Play sound effect

    if (health < 0) {
        health = 0; // Prevent negative health
    }
}

// Check if Geemer is dead
bool Geemer::isDead() const {
    return health <= 0; // Dead if health is 0 or less
}

// Check if Geemer is on a wall
bool Geemer::isOnWall() const {
    int currentX = static_cast<int>(x / room.getTileSize());
    int currentY = static_cast<int>(y / room.getTileSize());
    // Wall detection logic
    bool wallToLeft = room.isCellBlocked(currentX - 1, currentY);
    bool wallToRight = room.isCellBlocked(currentX + 1, currentY);
    return wallToLeft || wallToRight;
}
