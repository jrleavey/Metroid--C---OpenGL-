#ifndef ENEMY_H
#define ENEMY_H

#include "SoundManager.h"

// Base class for all enemy types
class Enemy {
public:
    virtual ~Enemy() {}

    // Pure virtual functions for common enemy behavior
    virtual void update(float deltaTime) = 0;
    virtual void render() const = 0;
    virtual void decreaseHealth(int damage) = 0;
    virtual bool isDead() const = 0;

    // Pure virtual functions for position and size
    virtual float getX() const = 0;
    virtual float getY() const = 0;
    virtual float getSize() const = 0;

    // Add other common methods as virtual

protected:
    int health; // Health of the enemy
    // Other common properties
};

#endif // ENEMY_H
