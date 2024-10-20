#include "AudioManager.h"
#include "Math.h"
#include <iostream>
#include <queue> 


AudioManager::AudioManager() {
    InitAudioDevice();
}
AudioManager::~AudioManager() {
    UnloadAllAudio();
    CloseAudioDevice();
}


void AudioManager::LoadMusic(const std::string& musicName, const std::string& filePath) {
    Music music = LoadMusicStream(filePath.c_str());
    musicTracks[musicName] = music;
}
void AudioManager::LoadSFX(const std::string& sfxName, const std::string& filePath) {
    Sound sfx = LoadSound(filePath.c_str());
    sfxFiles[sfxName] = sfx;
}
void AudioManager::LoadSpatialSFX(const std::string& sfxName, const std::string& filePath, int poolSize) {
    std::queue<Sound> soundPool;
    for (int i = 0; i < poolSize; i++) {
        Sound sfx = LoadSound(filePath.c_str());
        soundPool.push(sfx);
    }
    soundPools[sfxName] = soundPool;  // Store the pool of sounds
}




void AudioManager::PlayMusic(const std::string& musicName) {
    if (musicTracks.find(musicName) != musicTracks.end()) {
        if (currentMusic != musicName) {
            StopMusic();
            currentMusic = musicName;
        }
        PlayMusicStream(musicTracks[musicName]);
    }
    else {
        std::cerr << "Music " << musicName << " not found!" << std::endl;
    }
}
void AudioManager::StopMusic() {
    if (currentMusic != "") {
        StopMusicStream(musicTracks[currentMusic]);
    }
}
void AudioManager::SetMusicVolume(float volume) {
    if (currentMusic != "") {
        void SetMusicVolume(Music music, float volume);
    }
}
void AudioManager::PlaySFX(const std::string& sfxName) {
    if (sfxFiles.find(sfxName) != sfxFiles.end()) {
        PlaySound(sfxFiles[sfxName]);
    }
    else {
        std::cerr << "SFX " << sfxName << " not found!" << std::endl;
    }
}


//this one was tough
void AudioManager::PlaySpatialSFX(const std::string& sfxName, Vector2 listenerPos, Vector2 sourcePos, float minDistance, float maxDistance) {
    if (soundPools.find(sfxName) != soundPools.end() && !soundPools[sfxName].empty()) {
        //next available sound from pool
        Sound sfx = soundPools[sfxName].front();
        soundPools[sfxName].pop();

        //distance and vol
        float distance = Distance(listenerPos, sourcePos);
        float volume = 1.0f - (distance - minDistance) / (maxDistance - minDistance);
        volume = Clamp(volume, 0.0f, 0.2f);

        //pan based on relative X pos
        float deltaX = sourcePos.x - listenerPos.x;
        float screenWidth = GetScreenWidth();
        float pan = -deltaX / (screenWidth / 2);
        pan = Clamp(pan, -1.0f, 1.0f);

        if (volume > 0.0f) {
            SetSoundVolume(sfx, volume);
            SetSoundPan(sfx, pan);
            PlaySound(sfx);

            //return to the pool
            soundPools[sfxName].push(sfx);
        }
    }
    else {
        std::cerr << "No available sound in pool for " << sfxName << std::endl;
    }
}


void AudioManager::Update(Vector2 listenerPos) {
    if (currentMusic != "") {
        UpdateMusicStream(musicTracks[currentMusic]);
    }
}


void AudioManager::UnloadAllAudio() {
    
    for (auto& track : musicTracks) {
        UnloadMusicStream(track.second);
    }
    musicTracks.clear();

    
    for (auto& sfx : sfxFiles) {
        UnloadSound(sfx.second);
    }
    sfxFiles.clear();

    
    for (auto& sfx : spatialSfxFiles) {
        UnloadSound(sfx.second);
    }
    spatialSfxFiles.clear();
}
