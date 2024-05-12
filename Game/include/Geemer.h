#ifndef GEEMER_H
#define GEEMER_H

#include "Room.h"
#include "Enemy.h"
#include "glut.h"
#include "openglframework.h"  
#include "openGLStuff.h"
#include "SOIL.h"


// Geemer class, a specific type of enemy
class Geemer : public Enemy {
public:
    enum class Direction { LEFT, RIGHT, UP, DOWN };

    // Constructor
    Geemer(float x, float y, float size, float speed, Room& room);

    // Override functions from Enemy class
    void update(float deltaTime) override;
    void render() const override;
    float getX() const override;
    float getY() const override;
    float getSize() const override;
    void decreaseHealth(int damage) override;
    bool isDead() const override;

    // Additional behaviors specific to Geemer
    bool isOnWall() const;

private:
    float x, y; // Position
    float size; // Size of Geemer
    float speed; // Movement speed
    Room& room; // Reference to the room where Geemer is
    bool onGround; // Flag for ground contact
    Direction direction; // Movement direction

    // Private helper methods
    void rotate();
    bool isNextCellBlocked() const;
    bool isOnGround() const;
    bool isNextTileEmpty() const;
    void moveAlongSurface(float deltaTime);
    void adjustDirectionForMovement(int& nextX, int& nextY) const;
    bool isAtEdge() const;

    bool isClimbing; // Indicates if Geemer is currently climbing 
    GLuint GeemerTexture; // Texture ID for the Geemer sprite

    // Animation related properties
    int currentFrame;
    float animationTimer;
    const float frameDuration = 0.1f; // Duration of each frame in seconds
};

#endif // GEEMER_H
