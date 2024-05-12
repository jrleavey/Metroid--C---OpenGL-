#include "SoundManager.h"

// Global SoundManager instance
SoundManager gSoundManager;

bool SoundManager::Initialize() {
    return Sound::Init();
}

void SoundManager::LoadSounds() {
    soundIds[TITLE_THEME] = Sound::LoadSound("Sound/TitleTheme.wav");
    soundIds[SAMUS_FANFARE] = Sound::LoadSound("Sound/SamusFanFare.wav");
    soundIds[BRINSTAR_THEME] = Sound::LoadSound("Sound/BrinstarTheme.wav");
    soundIds[ITEM_ACQ_FANFARE] = Sound::LoadSound("Sound/ItemFanfare.wav");
    soundIds[POWER_BEAM] = Sound::LoadSound("Sound/PowerBeam.wav");
    soundIds[BOMB_EXPLODE] = Sound::LoadSound("Sound/BombExplode.wav");
    soundIds[BOMB_SPAWN] = Sound::LoadSound("Sound/BombSpawn.wav");
    soundIds[ENEMY_DAMAGE] = Sound::LoadSound("Sound/EnemyDamage.wav");
    soundIds[JUMP] = Sound::LoadSound("Sound/Jump.wav");
    soundIds[MISSILE_SHOOT] = Sound::LoadSound("Sound/MissileShoot.wav");
    soundIds[RIPPER_IMMUNE] = Sound::LoadSound("Sound/RipperImmune.wav");
    soundIds[PICKUP] = Sound::LoadSound("Sound/Pickup.wav");
    soundIds[SAMUS_FOOTSTEP] = Sound::LoadSound("Sound/SamusFootstep.wav");
    soundIds[SAMUS_MORPH] = Sound::LoadSound("Sound/SamusMorph.wav");
    soundIds[TAKE_DAMAGE] = Sound::LoadSound("Sound/TakeDamage.wav");

}

void SoundManager::PlaySoundW(SoundID id) {
    if (id >= 0 && id < SOUND_COUNT) {
        Sound::Playback(soundIds[id]);
    }
}
void SoundManager::StopSound(SoundID id) {
    if (id >= 0 && id < SOUND_COUNT) {
        Sound::StopPlayback(soundIds[id]);
    }
}
void SoundManager::Shutdown() {
    for (int i = 0; i < SOUND_COUNT; ++i) {
        Sound::UnloadSound(soundIds[i]);
    }
    Sound::Shutdown();
}