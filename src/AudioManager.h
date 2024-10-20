#pragma once
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <queue> 
#include <map>


class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void LoadMusic(const std::string& musicName, const std::string& filePath);
    void LoadSFX(const std::string& sfxName, const std::string& filePath);
    void LoadSpatialSFX(const std::string& sfxName, const std::string& filePath, int poolSize);
    

    void PlayMusic(const std::string& musicName);
    void PlaySFX(const std::string& sfxName);
    void PlaySpatialSFX(const std::string& sfxName, Vector2 listenerPos, Vector2 sourcePos, float minDistance = 1.0f, float maxDistance=300);

    void Update(Vector2 listenerPos);

    void StopMusic();
    void SetMusicVolume(float volume);

    void UnloadAllAudio();

private:
    std::unordered_map<std::string, Music> musicTracks;  //store music files
    std::unordered_map<std::string, Sound> sfxFiles;     //basic sfx
    std::unordered_map<std::string, Sound> spatialSfxFiles; //spatial sfx
    std::map<std::string, std::queue<Sound>> soundPools;//raylib dont support multiple sounds at once so i did it this way

    std::string currentMusic;
};

#endif
