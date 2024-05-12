#ifndef POWERUP_H
#define POWERUP_H

#include "Room.h"
#include "Enemy.h"
#include "Player.h"
#include "glut.h"
#include "SOIL.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Abstract class representing a generic powerup in the game
class Powerup {
public:
    // Constructor for creating a Powerup
    Powerup(float x, float y, float size);

    // Pure virtual functions for subclasses to define specific behavior
    virtual void applyEffect(Player& player) = 0;
    virtual void render() = 0;

    // Update function for handling animations
    void update(float deltaTime);

    // Get the position of the powerup
    Position getPosition() const;

protected:
    Position position; // Position of the powerup
    float tileSize;    // Size of the powerup

    // Animation related members
    GLuint powerupTexture; // Texture for the powerup sprites
    int currentFrame;      // Current frame in the animation
    float animationTimer;  // Timer for controlling animation speed
    float frameDuration;   // Duration of each frame in the animation

    // Helper method for rendering specific frames
    void render(int frameStart);
};

class MorphBallPowerup : public Powerup {
public:
    MorphBallPowerup(float x, float y, float size);
    void applyEffect(Player& player) override;
    void render() override;
};

class MissileExpansionPowerup : public Powerup {
public:
    MissileExpansionPowerup(float x, float y, float size);
    void applyEffect(Player& player) override;
    void render() override;
};

class LongBeamPowerup : public Powerup {
public:
    LongBeamPowerup(float x, float y, float size);
    void applyEffect(Player& player) override;
    void render() override;
};

class EnergyTankPowerup : public Powerup {
public:
    EnergyTankPowerup(float x, float y, float size);
    void applyEffect(Player& player) override;
    void render() override;
};

class MorphBallBombsPowerup : public Powerup {
public:
    MorphBallBombsPowerup(float x, float y, float size);
    void applyEffect(Player& player) override;
    void render() override;
};

#endif // POWERUP_H
