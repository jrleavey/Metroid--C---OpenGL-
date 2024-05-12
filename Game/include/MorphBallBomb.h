#ifndef MORPHBALLBOMB_H
#define MORPHBALLBOMB_H

#include "Player.h" // Include the Player header
#include "Room.h"   // Include the Room header
#include "Enemy.h"  // Include the Enemy header (if used)
#include "openglframework.h"  
#include "openGLStuff.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Class representing MorphBallBomb
class MorphBallBomb {
public:
    // Define the Position struct within MorphBallBomb class
    struct BombPosition {
        float x;
        float y;
        BombPosition(float _x, float _y) : x(_x), y(_y) {}
    };

    static const float BOMB_SIZE; // Constant representing the bomb size

    // Constructor
    MorphBallBomb(float x, float y, float size, Player& player);

    // Update method for the bomb
    void update(float deltaTime, const Room& currentRoom, std::vector<std::unique_ptr<Enemy>>& enemies);

    // Check if the bomb has exploded
    bool isExploded() const;

    // Check collision with the player
    bool collidesWithPlayer(const Player& player) const;

    // Render the bomb
    void render() const;

    // Getters for position and size
    float getX() const;
    float getY() const;
    float getSize() const;

    // Methods to mark and check if the bomb is marked for removal
    void markForRemoval();
    bool isMarkedForRemoval() const;

private:
    BombPosition position; // Position of the bomb
    float size; // Size of the bomb
    bool exploded; // Flag indicating if the bomb has exploded
    float explosionRadius; // Explosion radius
    float explosionTimer; // Timer for explosion
    Player& player; // Reference to the player object
    bool markedForRemoval = false; // Flag to indicate if the bomb should be removed
};

#endif // MORPHBALLBOMB_H
