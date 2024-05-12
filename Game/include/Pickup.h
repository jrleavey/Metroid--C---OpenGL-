#ifndef PICKUP_H
#define PICKUP_H

#include "Room.h"
#include "glut.h"
#include "openglframework.h"  
#include "openGLStuff.h"

// Class representing pickup items in the game
class Pickup {
public:
    // Enumeration for the type of pickup (Energy or Missile)
    enum Type { Energy, Missile };

    // Constructor for Pickup class
    Pickup(Type type, float x, float y, float size, float duration);

    // Update method for Pickup, called every frame with the delta time
    void update(float deltaTime);

    // Render method for Pickup, used to draw the pickup on the screen
    void render() const;

    // Check if the pickup's lifespan has expired
    bool isExpired() const;

    // Getter method to return the type of the pickup
    Type getType() const;

    // Mark the pickup for removal from the game
    void markForRemoval();

    // Check if the pickup is marked for removal
    bool isMarkedForRemoval() const;

    // Getter methods for the position and size of the pickup
    float getX() const;
    float getY() const;
    float getSize() const;

private:
    Type type; // The type of the pickup (Energy or Missile)
    float x, y; // X and Y coordinates of the pickup's position
    float size; // Size of the pickup
    float lifespan; // The total lifespan of the pickup (duration it stays active)
    float timer; // Timer to track how long the pickup has been active
    bool markedForRemoval; // Flag to indicate whether the pickup is to be removed
};

#endif // PICKUP_H
