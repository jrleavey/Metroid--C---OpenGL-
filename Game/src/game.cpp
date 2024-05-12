#define GAME_CPP
#include <windows.h>                                            // Header File For Windows
#include <stdio.h>                                              // Header File For Standard Input / Output
#include <stdarg.h>                                             // Header File For Variable Argument Routines
#include <math.h>                                               // Header File For Math Operations
#include <gl\gl.h>                                              // Header File For The OpenGL32 Library
#include <gl\glu.h>                                             // Header File For The GLu32 Library
#include "glut.h"
#include "baseTypes.h"
#include "openglframework.h"    
#include "gamedefs.h"
#include "collInfo.h"
#include "object.h"
#include "inputmapper.h"
#include "ball.h"
#include "field.h"
#include "random.h"
#include "gameObjects.h"
#include "openGLStuff.h"
#include "StateManager.h"
#include "BallManager.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "SpriteDemoManager.h"
#include "igame.h"
#include "Pickup.h"
#include "Game.h"
#include <algorithm>
#include "Camera.h" // Include Camera header
#include <string> // Include for std::string and std::to_string
#include <iostream>
#include "SOIL.h"
#include "SoundManager.h"

// avoid global definitions. these should be private members of your game class, or managed by another class.
//  pass in references (as you have done w/ "current room" when code needs to operate on them. this makes
//  your code safe, maintainable and re-entrant... and also limits unanticipated side-effects

Player* player; // Global definition of the Player object
Room* currentRoom; // Global definition of the current Room object
Camera* camera; // Global definition of the Camera object

// rampant usage of STL... understanding that it helped to you really push the scope of your game in other
//  areas, part of the goal of this assigment was to gain comfort and respect for other programming patterns
//  you will encounter outside of managed environments, and especially in situations where memory allocation
//  must be intentional (real-time programming)

std::vector<Projectile> projectiles; // Container for active projectiles
std::vector<Powerup*> powerups;  // Add this line to store powerups
std::vector<std::unique_ptr<MorphBallBomb>> bombs;

constexpr float tileSize = 64.0f; // Tile size


void CreateDebugConsole() {
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
    std::cout << "Debug Console Initialized" << std::endl;
}

void CGame::keyProcess() {
    if (isTitleScreenActive) {
        if (g_keys->keyDown[VK_RETURN]) {
            isTitleScreenActive = false;
        }
    }

    // Movement handling
    if (g_keys->keyDown['A']) {
        player->moveLeft(0.016f, *currentRoom);
        player->setIsMoving(true);
    }
    else if (g_keys->keyDown['D']) {
        player->moveRight(0.016f, *currentRoom);
        player->setIsMoving(true);
    }
    else {
        player->setIsMoving(false); // Set isMoving to false if neither left nor right keys are pressed
    }

    // Update isFacingUp based on 'W' key
    player->setIsFacingUp(g_keys->keyDown['W']); // Set isFacingUp to true only when 'W' is pressed

    // Toggle Morph Ball mode with cooldown check
    if (g_keys->keyDown['B'] && player->canToggleMorphBall()) {
        player->toggleMorphBall();
    }

    // Check for Morph Ball Bomb spawning
    if (g_keys->keyDown['J'] && player->isInMorphBallMode() && player->hasAcquiredMorphBallBombs) {
        if (bombs.size() < 3 && timeSinceLastBomb >= 0.25) {
            std::cout << "Spawn Bomb" << std::endl;
            float bombX = player->getX();
            float bombY = player->getY();
            auto newBomb = std::make_unique<MorphBallBomb>(bombX, bombY, MorphBallBomb::BOMB_SIZE, *player);
            bombs.push_back(std::move(newBomb));
            timeSinceLastBomb = 0.0f;
            gSoundManager.PlaySoundW(BOMB_SPAWN);

        }
    }

    // Actions for when not in Morph Ball mode
    if (!player->isInMorphBallMode()) {
        if (g_keys->keyDown[VK_SPACE] && player->isOnGround(*currentRoom)) {
            player->jump(0.016f, *currentRoom);
        }

        if (g_keys->keyDown['J']) {
            // Handle regular shooting behavior when not in Morph Ball mode
            float projectileX = player->getX();
            // Adjust the Y position where the projectile spawns
            float projectileYOffset = player->getSize().second * .9f; // Adjust this value as needed
            float projectileY = player->getY() + player->getSize().second - projectileYOffset;
            int projectileDirection = player->getFacingDirection();

            if (g_keys->keyDown['W']) {
                projectileDirection = 0; // Upward
            }

            if (player->isUsingMissiles()) {
                if (player->canFireMissile() && timeSinceLastShot >= Projectile::MISSILE_FIRE_RATE) {
                    Projectile newMissile(projectileX, projectileY, Projectile::MISSILE_SIZE, Projectile::MISSILE_SPEED,
                        Projectile::MISSILE_RANGE, Projectile::MISSILE_DAMAGE, projectileDirection, true);
                    projectiles.push_back(newMissile);
                    player->consumeMissile();
                    timeSinceLastShot = 0.0f;
                    gSoundManager.PlaySoundW(MISSILE_SHOOT);

                }
            }
            else if (timeSinceLastShot >= Projectile::DEFAULT_PROJECTILE_FIRE_RATE) {
                float projectileRange = player->hasAcquiredLongBeam ? INFINITY : Projectile::DEFAULT_RANGE;
                Projectile newProjectile(projectileX, projectileY, Projectile::DEFAULT_SIZE, Projectile::DEFAULT_SPEED,
                    projectileRange, Projectile::DEFAULT_DAMAGE, projectileDirection, false);
                projectiles.push_back(newProjectile);
                timeSinceLastShot = 0.0f;
                gSoundManager.PlaySoundW(POWER_BEAM);
            }
        }
    }

    if (g_keys->keyDown['M']) {
        if (player->getMissileAmmo() > 0 || player->isUsingMissiles()) {
            player->toggleMissiles();
        }
    }

    if (player->isUsingMissiles() && player->getMissileAmmo() <= 0) {
        player->toggleMissiles();
    }

    if (g_keys->keyDown[VK_ESCAPE])
    {
        TerminateApplication(g_window);
    }
    if (g_keys->keyDown[VK_F1]) {
        ToggleFullscreen(g_window);
    }
}





IGame* CreateGame() {
    return new CGame();

}

void DestroyGame(IGame* pGame) {
    delete pGame;
}

const char8_t CGame::mGameTitle[] = "Framework1";

CGame::CGame() {
    CreateDebugConsole();
    currentRoom = nullptr;
}




CGame::~CGame() {
}

// Initialize the game
bool CGame::Initialize(GL_Window* window, Keys* keys) {
    // Create and load the game's room
    currentRoom = new Room(480, 250, tileSize);
    currentRoom->loadRoomFromFile(".\\Room.txt Files\\Brinstar.txt");

    // Place power-ups and enemies in the game
    PlacePowerUps();
    PlaceEnemies();

    // Initialize the sound manager and load sounds
    
    // prefer singleton pattern to global instance, for lifetime management & avoidance of static initialization issues
    gSoundManager.Initialize();
    gSoundManager.LoadSounds();

    // Set up the camera
    camera = new Camera(400, 400);
    camera->update(*player, *currentRoom);
    camera->applyViewTransform();

    // Initialize OpenGL drawing parameters
    initOpenGLDrawing(window, keys, 0.0f, 0.0f, 0.0f);

    // Load the title screen texture
    titleTexture = SOIL_load_OGL_texture("asset/Title Screen.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    if (titleTexture == 0) {
        std::cerr << "Failed to load title texture: " << SOIL_last_result() << std::endl;
    }

    // Initialize title screen animation parameters
    titleFrame = 1;
    titleAnimationTimer = 0.0f;
    titleFrameDuration = 0.16f; // Adjust for desired frame change speed

    // Activate the title screen
    isTitleScreenActive = true;

    return true;
}
// Clean up the game
void CGame::Deinitialize() {
    // Delete player and camera objects
    delete player;
    delete camera;

    // Delete all power-ups
    for (auto& powerup : powerups) {
        delete powerup;
    }

    // Delete the room and shut down the sound manager
    delete currentRoom;
    currentRoom = nullptr;
    gSoundManager.Shutdown();
}

void CGame::UpdateFrame(uint32_t milliseconds) {
    static bool isPaused = false;
    static float pauseTimer = 0.0f;
    timeSinceLastBomb += milliseconds / 1000.0f;
    float deltaTime = milliseconds / 1000.0f;


    // Handling the title screen animation and transition
    if (isTitleScreenActive) {
        titleAnimationTimer += milliseconds / 1000.0f;
        if (titleAnimationTimer >= titleFrameDuration) {
            titleFrame = (titleFrame + 1) % 170; // Cycle through 170 frames
            titleAnimationTimer -= titleFrameDuration;
            if (!isMainMenuThemePlaying) {
                gSoundManager.PlaySound(TITLE_THEME);
                isMainMenuThemePlaying = true;
            }
        }

        // Check for user input to start the game
        if (g_keys->keyDown[VK_RETURN]) {
            gSoundManager.StopSound(TITLE_THEME);
            isTitleScreenActive = false;
            gameStartDelay = 7.0f; // Start 7 seconds countdown
        }
        return; // Skip the rest of the game update logic while on the title screen
    }
    //Samus Spawns in Fanfare
    if (!isSamusFanfarePlaying) {
        gSoundManager.PlaySound(SAMUS_FANFARE);
        isSamusFanfarePlaying = true;
    }

    // Handling the game start delay after the title screen
    if (gameStartDelay > 0.0f) {
        gameStartDelay -= milliseconds / 1000.0f;
        if (gameStartDelay <= 0.0f) {
            gameStartDelay = 0.0f;
        }
        else {
            return; // Continue pausing the game until the delay is over
        }
    }

    if (isPaused) {
        pauseTimer -= milliseconds / 1000.0f;
        if (pauseTimer <= 0.0f) {
            isPaused = false;
            if (isItemFanfarePlaying) {
                gSoundManager.StopSound(ITEM_ACQ_FANFARE);
                isItemFanfarePlaying = false;
            }
            if (!isBrinstarThemePlaying) {
                gSoundManager.PlaySound(BRINSTAR_THEME);
                isBrinstarThemePlaying = true;
            }
        }
        return; // Skip updating the game state if paused
    }

    // Regular game update logic
    gSoundManager.StopSound(SAMUS_FANFARE);

    if (!isBrinstarThemePlaying) {
        gSoundManager.PlaySound(BRINSTAR_THEME);
        isBrinstarThemePlaying = true;
    }
    keyProcess();
    player->update(deltaTime, *currentRoom);
    player->applyGravity(deltaTime, *currentRoom);
    camera->update(*player, *currentRoom);
    camera->applyViewTransform();
    checkProjectileEnemyCollisions(); // Check and handle collisions between projectiles and enemies

    for (auto it = powerups.begin(); it != powerups.end();) {
        if (checkCollision(*player, **it)) {
            (*it)->applyEffect(*player);

            gSoundManager.StopSound(BRINSTAR_THEME);
            isBrinstarThemePlaying = false;

            if (!isItemFanfarePlaying) {
                gSoundManager.PlaySound(ITEM_ACQ_FANFARE);
                isItemFanfarePlaying = true;
            }

            isPaused = true;
            pauseTimer = 6.0f; // Set pause duration for 6 seconds

            delete* it; // Delete the powerup and remove from memory
            it = powerups.erase(it);
        }
        else {
            ++it;
        }
    }
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);

        if (checkCollision(*player, *enemy)) {
            player->decreaseHealth(5); // Adjust damage as needed
        }
    }

    timeSinceLastShot += deltaTime; // Update the time since the last shot was fired
    player->updateIframes(deltaTime); // Update invincibility frames

    // Update and render projectiles
    for (auto& projectile : projectiles) {
        projectile.update(deltaTime);

        // Check collision with impassable tiles
        if (checkProjectileTileCollision(projectile, *currentRoom)) {
            projectile.markForRemoval();  // Mark the projectile for removal
        }

        projectile.render();
    }

    // Remove expired or collided projectiles
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& p) { return p.isExpired() || p.isMarkedForRemoval(); }),
        projectiles.end());

    // Update and render pickups
    for (auto& pickup : pickups) {
        pickup.update(deltaTime);

        if (checkCollision(*player, pickup)) {
            handlePickupCollection(*player, pickup);
            gSoundManager.PlaySoundW(PICKUP);

            pickup.markForRemoval();
        }

        pickup.render();
    }

    // Update and manage MorphBallBombs
    for (auto& bomb : bombs) {
        bomb->update(deltaTime, *currentRoom, enemies);

        // Check if the bomb has exploded and needs to be removed
        if (bomb->isExploded() || bomb->isMarkedForRemoval()) {
            bomb->markForRemoval();
        }
    }

    // Remove expired MorphBallBombs
    bombs.erase(std::remove_if(bombs.begin(), bombs.end(),
        [](const auto& bomb) { return bomb->isMarkedForRemoval(); }),
        bombs.end());

    // Remove expired pickups
    pickups.erase(std::remove_if(pickups.begin(), pickups.end(),
        [](const Pickup& p) { return p.isExpired() || p.isMarkedForRemoval(); }),
        pickups.end());

    // Calculate the bounding box of the player
    float playerLeft = player->getX() - player->getSize().first / 2;
    float playerRight = player->getX() + player->getSize().first / 2;
    float playerTop = player->getY() + player->getSize().second / 2;
    float playerBottom = player->getY() - player->getSize().second / 2;

}


void CGame::handlePickupCollection(Player& player, const Pickup& pickup) {
    if (pickup.getType() == Pickup::Energy) {
        player.increaseHealth(10);
    }
    else if (pickup.getType() == Pickup::Missile) {
        player.increaseMissileAmmo(5);
    }
}

//THIS FUNCTION UPDATES GRAPHICS. MAKE SURE EACH RENDER IS CALLED HERE
void CGame::DrawScene() {
    startOpenGLDrawing();

    if (isTitleScreenActive) {
        // Make sure texturing is enabled
        glEnable(GL_TEXTURE_2D);

        // Bind the title screen texture
        glBindTexture(GL_TEXTURE_2D, titleTexture);

        // Calculate texture coordinates for the current frame
        float texWidth = 520.0f / 88400.0f; // Width of one frame relative to the total texture width
        float texLeft = titleFrame * texWidth;
        float texRight = texLeft + texWidth;

        float quadSize = 1000.0f; // Adjust this value as needed

        glBegin(GL_QUADS);
        glTexCoord2f(texLeft, 1.0f); glVertex2f(-quadSize, -quadSize); // Bottom Left
        glTexCoord2f(texRight, 1.0f); glVertex2f(quadSize, -quadSize); // Bottom Right
        glTexCoord2f(texRight, 0.0f); glVertex2f(quadSize, quadSize); // Top Right
        glTexCoord2f(texLeft, 0.0f); glVertex2f(-quadSize, quadSize); // Top Left
        glEnd();

        // Unbind the texture after use
        glBindTexture(GL_TEXTURE_2D, 0);

        // Disable texturing
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        camera->applyViewTransform();

        currentRoom->render();
        player->render();

        // Update each powerup
        for (auto& powerup : powerups) {
            powerup->update(.01f);
        }

        // Render each powerup
        for (auto& powerup : powerups) {
            powerup->render();
        }
        // Render all enemies
        for (const auto& enemy : enemies) {
            enemy->render();
        }

        // Render Projectiles
        for (const Projectile& projectile : projectiles) {
            projectile.render();
        }

        // Render Pickups
        for (const Pickup& pickup : pickups) {
            pickup.render();
        }
        // Render MorphBallBombs
        for (const auto& bomb : bombs) {
            bomb->render();
        }
        DrawHealthBar(*player, 1024, 768);
        DrawMissileIndicator(*player, 1024, 768);
    }
}

void DrawHealthBar(const Player& player, int screenWidth, int screenHeight) {
    // Save the current OpenGL state
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_DEPTH_BUFFER_BIT); // Save the depth test state
    glDisable(GL_DEPTH_TEST); // Disable depth test for 2D overlay

    // Draw the background of the health bar (gray bar)
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color for the background
    glBegin(GL_QUADS);
    glVertex2f(10, screenHeight - 50); // Top left
    glVertex2f(210, screenHeight - 50); // Top right
    glVertex2f(210, screenHeight - 30); // Bottom right
    glVertex2f(10, screenHeight - 30); // Bottom left
    glEnd();

    // Calculate the width of the current health
    float healthPercentage = static_cast<float>(player.getHealth()) / player.getMaxHealth();
    float currentHealthWidth = 200.0f * healthPercentage;

    // Draw the foreground of the health bar (red bar)
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the health
    glBegin(GL_QUADS);
    glVertex2f(10, screenHeight - 50); // Top left
    glVertex2f(10 + currentHealthWidth, screenHeight - 50); // Top right
    glVertex2f(10 + currentHealthWidth, screenHeight - 30); // Bottom right
    glVertex2f(10, screenHeight - 30); // Bottom left
    glEnd();


    int energyTanks = player.getEnergyTanks();
    for (int i = 0; i < energyTanks; ++i) {
        float x = 220 + (i * 15); // Adjusted position and gap
        float y = screenHeight - 10; // Adjusted position above the health bar

        // Determine if the tank is full or empty
        bool isTankFull = player.energyTankFull[i];

        // Always draw the outline of the Energy Tank
        glColor3f(1.0f, 1.0f, 1.0f); // White color for tank outline
        glLineWidth(1.5f); // Adjusted line width
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + 10, y); // Top Right
        glVertex2f(x + 10, y - 10); // Bottom Right
        glVertex2f(x, y - 10); // Bottom Left
        glEnd();

        // Fill the box if the tank is full
        if (isTankFull) {
            glColor3f(1.0f, 1.0f, 1.0f); // Green color for filled tank
            glLineWidth(1.0f);
            for (float fillY = y - 1; fillY > y - 10; fillY--) {
                glBegin(GL_LINES);
                glVertex2f(x, fillY);
                glVertex2f(x + 10, fillY);
                glEnd();
            }
        }
    }

    // Restore the depth test state and original matrices
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
void DrawMissileIndicator(const Player& player, int screenWidth, int screenHeight) {
    int missileCount = player.getMissileAmmo();
    int iconSize = 20; // Size of each missile icon
    int padding = 5; // Space between each icon

    // Save the current OpenGL state
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_BUFFER_BIT); // Save the depth test state
    glDisable(GL_DEPTH_TEST); // Disable depth test for 2D overlay

    // Starting position for the first icon
    int startX = 10;
    int startY = screenHeight - 80; // Position it below the health bar

    // Set color for the missile icons (e.g., blue)
    glColor3f(0.0f, 0.0f, 1.0f);

    // Draw missile icons
    for (int i = 0; i < missileCount; i++) {
        glBegin(GL_QUADS);
        glVertex2f(startX + (iconSize + padding) * i, startY);
        glVertex2f(startX + iconSize + (iconSize + padding) * i, startY);
        glVertex2f(startX + iconSize + (iconSize + padding) * i, startY + iconSize);
        glVertex2f(startX + (iconSize + padding) * i, startY + iconSize);
        glEnd();
    }

    // Restore the depth test state and original matrices
    glPopAttrib();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void CGame::spawnRipper(int gridX, int gridY, float size, float speed) {
    // Convert grid coordinates to world coordinates
    Position worldPos = currentRoom->gridToWorld(gridX, gridY);

    // Adjust the position to center the Ripper in the tile
    worldPos.x += currentRoom->getTileSize() / 2.0f;
    worldPos.y += currentRoom->getTileSize() / 2.0f;

    // Create and add a new Ripper to the enemies vector
    enemies.push_back(std::make_unique<Ripper>(worldPos.x, worldPos.y, size, speed, *currentRoom));

}
void CGame::spawnSkree(int gridX, int gridY, float size, float speed) {
    Position worldPos = currentRoom->gridToWorld(gridX, gridY);
    worldPos.x += currentRoom->getTileSize() / 2.0f;
    worldPos.y += currentRoom->getTileSize() / 2.0f;

    auto skree = std::make_unique<Skree>(worldPos.x, worldPos.y, size, speed, *currentRoom);
    skree->setPlayer(*player);  // Set the player reference
    enemies.push_back(std::move(skree));

}

void CGame::spawnGeemer (int gridX, int gridY, float size, float speed) {
    // Convert grid coordinates to world coordinates
    Position worldPos = currentRoom->gridToWorld(gridX, gridY);

    // Adjust the position to center the Ripper in the tile
    worldPos.x += currentRoom->getTileSize() / 2.0f;
    worldPos.y += currentRoom->getTileSize() / 2.0f;

    // Create and add a new Ripper to the enemies vector
    enemies.push_back(std::make_unique<Geemer>(worldPos.x, worldPos.y, size, speed, *currentRoom));

}


bool CGame::checkCollision(const Projectile& projectile, const Enemy& enemy) {
    // AABB collision detection logic
    // Define the bounding boxes for the projectile and the enemy
    // Check if the boxes overlap and return true if they do
    // Example:
    float projectileLeft = projectile.getX() - projectile.getSize() / 2;
    float projectileRight = projectile.getX() + projectile.getSize() / 2;
    float projectileTop = projectile.getY() + projectile.getSize() / 2;
    float projectileBottom = projectile.getY() - projectile.getSize() / 2;

    float enemyLeft = enemy.getX() - enemy.getSize() / 2;
    float enemyRight = enemy.getX() + enemy.getSize() / 2;
    float enemyTop = enemy.getY() + enemy.getSize() / 2;
    float enemyBottom = enemy.getY() - enemy.getSize() / 2;

    return !(projectileLeft > enemyRight || projectileRight < enemyLeft ||
        projectileTop < enemyBottom || projectileBottom > enemyTop);
}
bool CGame::checkCollision(const Player& player, const Enemy& enemy) {
    // AABB collision detection logic
    float playerLeft = player.getX() - player.getSize().first / 2;
    float playerRight = player.getX() + player.getSize().first / 2;
    float playerTop = player.getY() + player.getSize().second / 2;
    float playerBottom = player.getY() - player.getSize().second / 2;

    float enemyLeft = enemy.getX() - enemy.getSize() / 2;
    float enemyRight = enemy.getX() + enemy.getSize() / 2;
    float enemyTop = enemy.getY() + enemy.getSize() / 2;
    float enemyBottom = enemy.getY() - enemy.getSize() / 2;

    return !(playerLeft > enemyRight || playerRight < enemyLeft ||
        playerTop < enemyBottom || playerBottom > enemyTop);
}
bool CGame::checkCollision(const Player& player, const Pickup& pickup) {
    // Calculate the bounding box of the player
    float playerLeft = player.getX() - player.getSize().first / 2;
    float playerRight = player.getX() + player.getSize().first / 2;
    float playerTop = player.getY() + player.getSize().second / 2;
    float playerBottom = player.getY() - player.getSize().second / 2;

    // Calculate the bounding box of the pickup
    float pickupLeft = pickup.getX() - pickup.getSize() / 2;
    float pickupRight = pickup.getX() + pickup.getSize() / 2;
    float pickupTop = pickup.getY() + pickup.getSize() / 2;
    float pickupBottom = pickup.getY() - pickup.getSize() / 2;

    // Check for overlap between the two bounding boxes
    return !(playerLeft > pickupRight || playerRight < pickupLeft ||
        playerTop < pickupBottom || playerBottom > pickupTop);
}
bool CGame::checkCollision(const Player& player, const Powerup& powerup) {
    // Calculate the bounding box of the player
    float playerLeft = player.getX() - player.getSize().first / 2;
    float playerRight = player.getX() + player.getSize().first / 2;
    float playerTop = player.getY() + player.getSize().second / 2;
    float playerBottom = player.getY() - player.getSize().second / 2;

    // Get the position of the powerup
    float powerupX = powerup.getPosition().x;
    float powerupY = powerup.getPosition().y;

    // Check if the powerup's position is within the player's bounding box
    return (powerupX >= playerLeft && powerupX <= playerRight &&
        powerupY >= playerBottom && powerupY <= playerTop);
}
bool CGame::checkCollision(const Player& player, const MorphBallBomb& bomb) {
    // AABB collision detection logic
    float playerLeft = player.getX() - player.getSize().first / 2;
    float playerRight = player.getX() + player.getSize().first / 2;
    float playerTop = player.getY() + player.getSize().second / 2;
    float playerBottom = player.getY() - player.getSize().second / 2;

    float bombLeft = bomb.getX() - bomb.getSize() / 2;
    float bombRight = bomb.getX() + bomb.getSize() / 2;
    float bombTop = bomb.getY() + bomb.getSize() / 2;
    float bombBottom = bomb.getY() - bomb.getSize() / 2;

    return !(playerLeft > bombRight || playerRight < bombLeft ||
        playerTop < bombBottom || playerBottom > bombTop);
}
bool CGame::checkProjectileTileCollision(const Projectile& projectile, const Room& room) {
    int gridX = static_cast<int>(projectile.getX() / room.getTileSize());
    int gridY = static_cast<int>(projectile.getY() / room.getTileSize());

    return room.isCellBlocked(gridX, gridY);
}
void CGame::checkProjectileEnemyCollisions() {
    for (auto& projectile : projectiles) {
        if (projectile.isExpired() || projectile.isMarkedForRemoval()) continue;

        for (auto& enemy : enemies) {
            if (checkCollision(projectile, *enemy)) {
                enemy->decreaseHealth(projectile.getDamage());
                projectile.markForRemoval();
                if (enemy->isDead()) {
                    // Determine pickup drop
                    float dropChance = static_cast<float>(rand()) / RAND_MAX;

                    if (dropChance < .2f) { // 20% chance for energy
                        pickups.push_back(Pickup(Pickup::Energy, enemy->getX(), enemy->getY(), 20.0f, 10.0f));
                    }
                    else if (dropChance < .4f) { // Additional 20% chance for missiles
                        pickups.push_back(Pickup(Pickup::Missile, enemy->getX(), enemy->getY(), 20.0f, 10.0f));
                    }

                    // Mark enemy for removal
                    // ...
                }
            }
        }
    }

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const auto& enemy) { return enemy->isDead(); }),
        enemies.end());
}
void CGame::PlacePowerUps()
{
    // Morph Ball powerup setup
    int morphBallPowerupGridX = 22;  // Example grid X position for the Morph Ball powerup
    int morphBallPowerupGridY = 44;   // Example grid Y position for the Morph Ball powerup
    Position morphBallPowerupWorldPos = currentRoom->gridToWorld(morphBallPowerupGridX, morphBallPowerupGridY);
    morphBallPowerupWorldPos.x += tileSize / 2.0f;
    morphBallPowerupWorldPos.y += tileSize / 2.0f;
    Powerup* morphBallPowerup = new MorphBallPowerup(morphBallPowerupWorldPos.x, morphBallPowerupWorldPos.y, tileSize);
    powerups.push_back(morphBallPowerup); // Add to the powerups vector


    // Morph Ball Bombs powerup setup
    int morphBallBombsPowerupGridX = 408;  // Example grid X position for the Morph Ball Bombs powerup
    int morphBallBombsPowerupGridY = 184;  // Example grid Y position for the Morph Ball Bombs powerup
    Position morphBallBombsPowerupWorldPos = currentRoom->gridToWorld(morphBallBombsPowerupGridX, morphBallBombsPowerupGridY);
    morphBallBombsPowerupWorldPos.x += tileSize / 2.0f;
    morphBallBombsPowerupWorldPos.y += tileSize / 2.0f;
    Powerup* morphBallBombsPowerup = new MorphBallBombsPowerup(morphBallBombsPowerupWorldPos.x, morphBallBombsPowerupWorldPos.y, tileSize);
    powerups.push_back(morphBallBombsPowerup);
    // Long Beam powerup setup
    int longBeamPowerupGridX = 105;  // Replace with the desired grid X position for the Long Beam powerup
    int longBeamPowerupGridY = 184;   // Replace with the desired grid Y position for the Long Beam powerup
    Position longBeamPowerupWorldPos = currentRoom->gridToWorld(longBeamPowerupGridX, longBeamPowerupGridY);
    longBeamPowerupWorldPos.x += tileSize / 2.0f;
    longBeamPowerupWorldPos.y += tileSize / 2.0f;
    Powerup* longBeamPowerup = new LongBeamPowerup(longBeamPowerupWorldPos.x, longBeamPowerupWorldPos.y, tileSize);
    powerups.push_back(longBeamPowerup); // Add to the powerups vector

        // Missile Expansion powerup 1
    {
        int missileExpPowerupGridX = 279;  // Example grid X position for the Missile Expansion powerup
        int missileExpPowerupGridY = 93;   // Example grid Y position for the Missile Expansion powerup
        Position missileExpPowerupWorldPos = currentRoom->gridToWorld(missileExpPowerupGridX, missileExpPowerupGridY);
        missileExpPowerupWorldPos.x += tileSize / 2.0f;
        missileExpPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* missileExpansionPowerup = new MissileExpansionPowerup(missileExpPowerupWorldPos.x, missileExpPowerupWorldPos.y, tileSize);
        powerups.push_back(missileExpansionPowerup); // Add to the powerups vector
    }
    // Missile Expansion powerup 2
    {
        int missileExpPowerupGridX = 313;  // Example grid X position for the Missile Expansion powerup
        int missileExpPowerupGridY = 155;   // Example grid Y position for the Missile Expansion powerup
        Position missileExpPowerupWorldPos = currentRoom->gridToWorld(missileExpPowerupGridX, missileExpPowerupGridY);
        missileExpPowerupWorldPos.x += tileSize / 2.0f;
        missileExpPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* missileExpansionPowerup = new MissileExpansionPowerup(missileExpPowerupWorldPos.x, missileExpPowerupWorldPos.y, tileSize);
        powerups.push_back(missileExpansionPowerup); // Add to the powerups vector
    }
    // Missile Expansion powerup 3
    {
        int missileExpPowerupGridX = 375;  // Example grid X position for the Missile Expansion powerup
        int missileExpPowerupGridY = 207;   // Example grid Y position for the Missile Expansion powerup
        Position missileExpPowerupWorldPos = currentRoom->gridToWorld(missileExpPowerupGridX, missileExpPowerupGridY);
        missileExpPowerupWorldPos.x += tileSize / 2.0f;
        missileExpPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* missileExpansionPowerup = new MissileExpansionPowerup(missileExpPowerupWorldPos.x, missileExpPowerupWorldPos.y, tileSize);
        powerups.push_back(missileExpansionPowerup); // Add to the powerups vector
    }
    {
        // Energy Tank powerup setup 1
        int energyTankPowerupGridX = 168;  // Position for the first Energy Tank powerup
        int energyTankPowerupGridY = 231;
        Position energyTankPowerupWorldPos = currentRoom->gridToWorld(energyTankPowerupGridX, energyTankPowerupGridY);
        energyTankPowerupWorldPos.x += tileSize / 2.0f;
        energyTankPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* energyTankPowerup = new EnergyTankPowerup(energyTankPowerupWorldPos.x, energyTankPowerupWorldPos.y, tileSize);
        powerups.push_back(energyTankPowerup);
    }

    {
        // Energy Tank powerup setup 2
        int energyTankPowerupGridX = 391;  // Position for the second Energy Tank powerup
        int energyTankPowerupGridY = 150;
        Position energyTankPowerupWorldPos = currentRoom->gridToWorld(energyTankPowerupGridX, energyTankPowerupGridY);
        energyTankPowerupWorldPos.x += tileSize / 2.0f;
        energyTankPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* energyTankPowerup = new EnergyTankPowerup(energyTankPowerupWorldPos.x, energyTankPowerupWorldPos.y, tileSize);
        powerups.push_back(energyTankPowerup);
    }

    {
        // Energy Tank powerup setup 3
        int energyTankPowerupGridX = 312;  // Position for the third Energy Tank powerup
        int energyTankPowerupGridY = 210;
        Position energyTankPowerupWorldPos = currentRoom->gridToWorld(energyTankPowerupGridX, energyTankPowerupGridY);
        energyTankPowerupWorldPos.x += tileSize / 2.0f;
        energyTankPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* energyTankPowerup = new EnergyTankPowerup(energyTankPowerupWorldPos.x, energyTankPowerupWorldPos.y, tileSize);
        powerups.push_back(energyTankPowerup);
    }
    { 
        // Energy Tank powerup setup 4
        int energyTankPowerupGridX = 423;  // Position for the third Energy Tank powerup
        int energyTankPowerupGridY = 205;
        Position energyTankPowerupWorldPos = currentRoom->gridToWorld(energyTankPowerupGridX, energyTankPowerupGridY);
        energyTankPowerupWorldPos.x += tileSize / 2.0f;
        energyTankPowerupWorldPos.y += tileSize / 2.0f;
        Powerup* energyTankPowerup = new EnergyTankPowerup(energyTankPowerupWorldPos.x, energyTankPowerupWorldPos.y, tileSize);
        powerups.push_back(energyTankPowerup);
    }


}
void CGame::PlaceEnemies()
{
    {

        // Player setup
        int playerGridX = 40; // Example grid X position for the player 40 is correct
        int playerGridY = 43;  // Example grid Y position for the player 43 is correct
        Position playerWorldPos = currentRoom->gridToWorld(playerGridX, playerGridY);
        player = new Player(playerWorldPos.x, playerWorldPos.y, 300.0f, 800.0f, 800.0f, tileSize);
        timeSinceLastBomb = 0.25f;
    }
    float enemySize = 50;
    float geemerSpeed = 200;
    float skreeSpeed = 600;
    float ripperSpeed = 140;
    //Geemers
    {
        //Corridor No. 1
        spawnGeemer(50, 41, enemySize, geemerSpeed);
        //Fake Block Shaft
        spawnGeemer(86, 48, enemySize, geemerSpeed);
        //Overhang Corridor
        spawnGeemer(105, 41, enemySize, geemerSpeed);
        spawnGeemer(117, 41, enemySize, geemerSpeed);
        //Blue Tinted Corridor
        spawnGeemer(170, 50, enemySize, geemerSpeed);
        spawnGeemer(164, 61, enemySize, geemerSpeed);
        spawnGeemer(170, 76, enemySize, geemerSpeed);
        spawnGeemer(164, 91, enemySize, geemerSpeed);
        spawnGeemer(170, 100, enemySize, geemerSpeed);
        spawnGeemer(164, 120, enemySize, geemerSpeed);
        spawnGeemer(164, 130, enemySize, geemerSpeed);
        spawnGeemer(170, 145, enemySize, geemerSpeed);
        spawnGeemer(170, 150, enemySize, geemerSpeed);
        spawnGeemer(164, 156, enemySize, geemerSpeed);
        spawnGeemer(164, 171, enemySize, geemerSpeed);
        spawnGeemer(170, 171, enemySize, geemerSpeed);
        spawnGeemer(164, 181, enemySize, geemerSpeed);
        spawnGeemer(164, 191, enemySize, geemerSpeed);
        spawnGeemer(170, 191, enemySize, geemerSpeed);
        spawnGeemer(164, 201, enemySize, geemerSpeed);
        spawnGeemer(170, 212, enemySize, geemerSpeed);
        spawnGeemer(164, 221, enemySize, geemerSpeed);
        //Long Beam Corridor
        spawnGeemer(134, 178, enemySize, geemerSpeed);
        spawnGeemer(124, 178, enemySize, geemerSpeed);
        //Gold Tinted Shaft
        spawnGeemer(196, 91, enemySize, geemerSpeed);
        spawnGeemer(203, 100, enemySize, geemerSpeed);
        spawnGeemer(197, 120, enemySize, geemerSpeed);
        spawnGeemer(199, 130, enemySize, geemerSpeed);
        spawnGeemer(205, 145, enemySize, geemerSpeed);
        spawnGeemer(204, 151, enemySize, geemerSpeed);
        spawnGeemer(199, 156, enemySize, geemerSpeed);
        spawnGeemer(198, 171, enemySize, geemerSpeed);
        spawnGeemer(204, 171, enemySize, geemerSpeed);
        spawnGeemer(199, 181, enemySize, geemerSpeed);
        spawnGeemer(199, 191, enemySize, geemerSpeed);
        spawnGeemer(203, 191, enemySize, geemerSpeed);
        spawnGeemer(196, 201, enemySize, geemerSpeed);
        spawnGeemer(205, 212, enemySize, geemerSpeed);
        spawnGeemer(198, 221, enemySize, geemerSpeed);
        //Corridor No 5
        spawnGeemer(218, 87, enemySize, geemerSpeed);
        spawnGeemer(248, 91, enemySize, geemerSpeed);
        // Green Corridor
        spawnGeemer(220, 205, enemySize, geemerSpeed);
        spawnGeemer(241, 205, enemySize, geemerSpeed);
        spawnGeemer(251, 205, enemySize, geemerSpeed);
        spawnGeemer(255, 209, enemySize, geemerSpeed);
        spawnGeemer(290, 206, enemySize, geemerSpeed);
        spawnGeemer(310, 206, enemySize, geemerSpeed);
        spawnGeemer(330, 206, enemySize, geemerSpeed);
        // Long Corridor of Bubbles
        spawnGeemer(231, 149, enemySize, geemerSpeed);
        spawnGeemer(245, 149, enemySize, geemerSpeed);
        //Final Shaft
        spawnGeemer(472, 149, enemySize, geemerSpeed);
        spawnGeemer(471, 158, enemySize, geemerSpeed);
        spawnGeemer(473, 167, enemySize, geemerSpeed);
        spawnGeemer(471, 176, enemySize, geemerSpeed);
        spawnGeemer(472, 185, enemySize, geemerSpeed);
        //Bomb Corridor
        spawnGeemer(426, 187, enemySize, geemerSpeed);
        spawnGeemer(434, 187, enemySize, geemerSpeed);
        spawnGeemer(442, 187, enemySize, geemerSpeed);
        spawnGeemer(450, 187, enemySize, geemerSpeed);






    }
        //Skree
    {
         //Flake Block Shaft
        spawnSkree(56, 50, enemySize, skreeSpeed);
        spawnSkree(64, 50, enemySize, skreeSpeed);
        //Overhang Corridor
        spawnSkree(106, 50, enemySize, skreeSpeed);
        spawnSkree(112, 50, enemySize, skreeSpeed);
        spawnSkree(120, 50, enemySize, skreeSpeed);
        //Long Beam Corridor
        spawnSkree(124, 186, enemySize, skreeSpeed);
        spawnSkree(132, 186, enemySize, skreeSpeed);
        spawnSkree(140, 186, enemySize, skreeSpeed);
        spawnSkree(148, 186, enemySize, skreeSpeed);
        //Corridor No 5
        spawnSkree(218, 96, enemySize, skreeSpeed);
        spawnSkree(224, 96, enemySize, skreeSpeed);
        spawnSkree(232, 96, enemySize, skreeSpeed);
        spawnSkree(256, 96, enemySize, skreeSpeed);
        spawnSkree(264, 96, enemySize, skreeSpeed);
        //Long Corridor of Bubbles
        spawnSkree(299, 156, enemySize, skreeSpeed);
        spawnSkree(306, 156, enemySize, skreeSpeed);
        spawnSkree(315, 156, enemySize, skreeSpeed);
        spawnSkree(322, 156, enemySize, skreeSpeed);
        spawnSkree(330, 156, enemySize, skreeSpeed);
        spawnSkree(363, 156, enemySize, skreeSpeed);
        spawnSkree(371, 156, enemySize, skreeSpeed);
        spawnSkree(379, 156, enemySize, skreeSpeed);
        spawnSkree(402, 156, enemySize, skreeSpeed);
        spawnSkree(410, 156, enemySize, skreeSpeed);
        spawnSkree(418, 156, enemySize, skreeSpeed);
        spawnSkree(426, 156, enemySize, skreeSpeed);
        spawnSkree(434, 156, enemySize, skreeSpeed);
        spawnSkree(442, 156, enemySize, skreeSpeed);
        spawnSkree(450, 156, enemySize, skreeSpeed);
        spawnSkree(458, 156, enemySize, skreeSpeed);

        //Bomb Corridor
        spawnSkree(426, 187, enemySize, skreeSpeed);
        spawnSkree(434, 187, enemySize, skreeSpeed);
        spawnSkree(442, 187, enemySize, skreeSpeed);
        spawnSkree(450, 187, enemySize, skreeSpeed);








    }
    //Rippers
    {
        //Blue Tinted Shaft
        spawnRipper(166, 49, enemySize, ripperSpeed);
        spawnRipper(167, 59, enemySize, ripperSpeed);
        spawnRipper(164, 72, enemySize, ripperSpeed);
        spawnRipper(164, 100, enemySize, ripperSpeed);
        spawnRipper(163, 109, enemySize, ripperSpeed);
        spawnRipper(165, 120, enemySize, ripperSpeed);
        spawnRipper(165, 132, enemySize, ripperSpeed);
        spawnRipper(168, 141, enemySize, ripperSpeed);
        spawnRipper(168, 150, enemySize, ripperSpeed);
        spawnRipper(164, 162, enemySize, ripperSpeed);
        spawnRipper(165, 172, enemySize, ripperSpeed);
        // Gold Tinted Shaft
        spawnRipper(194, 100, enemySize, ripperSpeed);
        spawnRipper(193, 109, enemySize, ripperSpeed);
        spawnRipper(195, 120, enemySize, ripperSpeed);
        spawnRipper(195, 132, enemySize, ripperSpeed);
        spawnRipper(198, 141, enemySize, ripperSpeed);
        spawnRipper(198, 150, enemySize, ripperSpeed);
        spawnRipper(194, 162, enemySize, ripperSpeed);
        spawnRipper(197, 172, enemySize, ripperSpeed);
        spawnRipper(197, 182, enemySize, ripperSpeed);
        spawnRipper(198, 192, enemySize, ripperSpeed);
        spawnRipper(196, 202, enemySize, ripperSpeed);
        //Final Shaft
        spawnRipper(472, 149, enemySize, ripperSpeed);
        spawnRipper(471, 158, enemySize, ripperSpeed);
        spawnRipper(473, 167, enemySize, ripperSpeed);
        spawnRipper(471, 176, enemySize, ripperSpeed);
        spawnRipper(472, 185, enemySize, ripperSpeed);
        spawnRipper(474, 194, enemySize, ripperSpeed);
        spawnRipper(474, 203, enemySize, ripperSpeed);
        spawnRipper(474, 212, enemySize, ripperSpeed);

    }
}

