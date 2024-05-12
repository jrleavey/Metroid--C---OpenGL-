#ifndef GAME_H
#define GAME_H

#include "IGame.h"
#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "Geemer.h"
#include "Ripper.h"
#include "Projectile.h"
#include "Pickup.h"
#include <memory> // Include for smart pointers
#include <vector>
#include "Powerup.h"
#include "Skree.h"
#include "MorphBallBomb.h"



extern Player* player; // Global declaration of the Player object

void DrawHealthBar(const Player& player, int screenWidth, int screenHeight);
void DrawMissileIndicator(const Player& player, int screenWidth, int screenHeight);


class CGame : public IGame {
public:
    CGame();
    ~CGame();

    bool Initialize(GL_Window* window, Keys* keys);
    void Deinitialize();
    const char8_t* GetGameTitle() { return mGameTitle; }
    uint32_t GetScreenWidth() { return 1024; }
    uint32_t GetScreenHeight() { return 768; }
    uint32_t GetBitsPerPixel() { return 32; }
    void DrawScene();
    void UpdateFrame(uint32_t milliseconds);
    void spawnGeemer(int gridX, int gridY, float size, float speed);  // Updated function declaration
    void spawnRipper(int gridX, int gridY, float size, float speed);  // Updated function declaration
    void spawnSkree(int gridX, int gridY, float size, float speed);  // Add this line
    bool checkCollision(const Player& player, const Enemy& enemy);
    bool checkCollision(const Projectile& projectile, const Enemy& enemy);  
    bool checkProjectileTileCollision(const Projectile& projectile, const Room& room);
    void checkProjectileEnemyCollisions(); // Add this line
    bool checkCollision(const Player& player, const Pickup& pickup);
    bool checkCollision(const Player& player, const Powerup& powerup);  // Ensure proper syntax
    bool checkCollision(const Player& player, const MorphBallBomb& bomb);
    float timeSinceLastBomb;


    void handlePickupCollection(Player& player, const Pickup& pickup);
    void PlacePowerUps();
    void PlaceEnemies();

    GLuint titleTexture; // Texture for the title screen

    bool isTitleScreenActive;
    float gameStartDelay;
    int titleFrame; // Current frame of the title animation
    float titleAnimationTimer; // Timer to control frame switching
    float titleFrameDuration; // Duration of each frame in the animation


private:
    static const char8_t mGameTitle[20];
    Room* currentRoom; // Room object
    void keyProcess(); // Private method for handling key inputs
    float fireRate = 0.33f; // Time in seconds between shots
    float timeSinceLastShot; // Time elapsed since the last shot
    std::vector<std::unique_ptr<Enemy>> enemies; // Use smart pointers for automatic memory management
    std::vector<Pickup> pickups;
    bool isBrinstarThemePlaying = false;
    bool isMainMenuThemePlaying = false;
    bool isSamusFanfarePlaying = false;
    bool isItemFanfarePlaying = false;
};

#endif // GAME_H