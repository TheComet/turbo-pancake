#include "Sound.h"
#include <iostream>
#include <string>

Sound::Sound() : mSound(nullptr), channel(-1) { }

bool Sound::load(std::string path) {
    //load back button sounds
    mSound=std::shared_ptr<Mix_Chunk>(Mix_LoadWAV(path.c_str()),Mix_FreeChunk);
    if (mSound == nullptr) {
        std::cout<<"Failed to load sound effect from "<<path<<"! "<<Mix_GetError()<<std::endl;
        return false;
    }
    return true;
}

void Sound::play() {
    //-1 selects nearest available channel, 0 repeats the sound 0 times
	if(mSound != nullptr)
		channel = Mix_PlayChannel(-1,mSound.get(),0);
}

bool Sound::playing() {
	return Mix_Playing(channel) == 1;
}

Sound loadSound(std::string filename) {
	Sound s;
	s.load(filename);
	return s;
}

Sound::operator bool() {
    return !(mSound==nullptr);
}


Music::Music() : mSound(nullptr) { }

bool Music::load(std::string path) {
    //load back button sounds
    mSound=std::shared_ptr<Mix_Music>(Mix_LoadMUS(path.c_str()),Mix_FreeMusic);
    if (mSound == nullptr) {
        std::cout<<"Failed to load music from "<<path<<"! "<<Mix_GetError()<<std::endl;
        return false;
    }
    return true;
}

void Music::play(int ntimes) {
    Mix_PlayMusic(mSound.get(),ntimes);
}

void Music::fadeIn(int ms, int ntimes) {
    Mix_FadeInMusic(mSound.get(),ntimes,ms);
}

void Music::stop() {
    Mix_HaltMusic();
}

void Music::fadeOut(int ms) {
    Mix_FadeOutMusic(ms);
}

//returns true if SOME music is playing.
bool Music::playing() {
    return Mix_PlayingMusic()==1;
}

Music::operator bool() {
    return !(mSound==nullptr);
}