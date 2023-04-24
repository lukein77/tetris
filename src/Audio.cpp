#include "Audio.h"

/* Audio constructor */
Audio::Audio() {}

/* Audio destructor */
Audio::~Audio() {
    Mix_FreeMusic(music);
    clearAudioCache();

    Mix_Quit();
}


/// @brief Initialize SDL_mixer
/// @return True if initialization was successfull, false if there were errors
bool Audio::initialize() {
    // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
		return false;
    }

    // Set volume for all channels to 25%
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);

    return true;
}


/// @brief Loads a music file
/// @return true if load was successful, false if there were errors
bool Audio::loadMusic(std::string filename) {
    music = Mix_LoadMUS(("sound/"+filename).c_str());
    return (music != NULL);
}


/// @brief Play music already loaded
void Audio::playMusic() {
    if (music) Mix_PlayMusic(music, -1);
}

/// @brief Load music file and play it
void Audio::playMusic(std::string filename) {
    if (loadMusic(filename)) {
        playMusic(); 
    } 
}

// TODO pause resume

/// @brief Stop music
void Audio::stopMusic() {
    Mix_HaltMusic();
}

void Audio::clearAudioCache() {
    for (auto &s : audio_cache) {
        Mix_FreeChunk(s.second);
    }
    audio_cache.clear();
}

/// @brief Loads a sound effect file that hasn't been loaded yet and inserts it into the sound cache
/// @param filename Name of the file to load
/// @return Pointer to the Mix_Chunk containing the sound effect
Mix_Chunk *Audio::addSoundToCache(std::string filename) {
    Mix_Chunk *sound = Mix_LoadWAV(("sound/"+filename).c_str());
    
    if (sound) {
        audio_cache.insert(std::pair<std::string, Mix_Chunk*>(filename, sound));
    }

    return sound;
}



/// @brief Load sound effect and play it
/// @param filename Name of the sound file
void Audio::playSound(std::string filename) {

    // Searches for sound in cache, if not found loads from disk
    Mix_Chunk *sound = new Mix_Chunk;

    std::map <std::string, Mix_Chunk*> :: iterator it;
	it = audio_cache.find(filename);

	if (it != audio_cache.end()) {
		sound = it->second;
	} else {
		sound = addSoundToCache(filename);
	}

    // Plays the sound
    Mix_PlayChannel(-1, sound, 0);
}

void Audio::playLandSound() {
    int random = rand() % 19;
    playSound("land"+std::to_string(random)+".wav");
}

void Audio::playLineSound() {
    int random = rand() % 3;
    playSound("line"+std::to_string(random)+".mp3");
}
