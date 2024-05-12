#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "Sound#1.h" // Your existing sound system header

enum SoundID {
    TITLE_THEME,
    SAMUS_FANFARE,
    BRINSTAR_THEME,
    ITEM_ACQ_FANFARE,
    POWER_BEAM,
    BOMB_EXPLODE,
    BOMB_SPAWN,
    ENEMY_DAMAGE,
    JUMP,
    MISSILE_SHOOT,
    RIPPER_IMMUNE,
    PICKUP,
    SAMUS_FOOTSTEP,
    SAMUS_MORPH,
    TAKE_DAMAGE,
    SOUND_COUNT // Represents the total number of sounds
};

class SoundManager {
public:
    SoundManager() = default; // Default constructor
    ~SoundManager() = default; // Default destructor

    bool Initialize(); // Initialize the audio system
    void LoadSounds(); // Load all sound effects
    void PlaySoundW(SoundID id);
    void Shutdown(); // Shutdown the audio system
    void StopSound(SoundID id);  // Method to stop a specific sound


private:
    int soundIds[SOUND_COUNT]; // Array to store sound IDs
};

extern SoundManager gSoundManager; // Global SoundManager instance

#endif // SOUNDMANAGER_H
