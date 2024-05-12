#ifndef SKREE_H
#define SKREE_H

#include "Room.h"
#include "Enemy.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include "openglframework.h"  
#include "gamedefs.h"
#include "openGLStuff.h"
#include <iostream>
#include "Player.h"

class Skree : public Enemy {
public:
    Skree(float x, float y, float size, float speed, Room& room);
    void update(float deltaTime) override;
    void render() const override;
    void decreaseHealth(int damage) override;
    bool isDead() const override;
    float getX() const override;
    float getY() const override;
    void setPlayer(const Player& player);
    float getSize() const override;

private:
    float x, y;
    float size;
    float speed;
    Room& room;
    const Player* player;
    bool dead;
    bool diving; // Added this line for diving state
    float deathTimer;
    static constexpr float EXPLOSION_DELAY = 3.0f;
    bool onGround;  // Indicates if Skree is on the ground
    float explosionTimer;  // Timer for explosion delay

    bool isAbovePlayer() const;

    GLuint SkreeTexture; // Texture ID for the Skree sprite
    int currentFrame;
    float animationTimer;
    const float frameDuration; // Duration of each frame in seconds
};

#endif // SKREE_H
