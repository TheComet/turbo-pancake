#pragma once
#include <iostream>
#include <memory>
#include "SDL_mixer.h"
class Sound
{
    std::shared_ptr<Mix_Chunk> mSound;
public:
    Sound();

    bool load(std::string path);
    void play();  
    
    //So we can call if(sound). Sound converts to bool and is false whenever mSound==nullptr. 
    explicit operator bool();
};

class Music
{
    std::shared_ptr<Mix_Music> mSound;
public:
    Music();

    bool load(std::string path);
    void play(int ntimes=-1);
    void fadeIn(int ms=1000, int ntimes=-1);
    void stop();
    void fadeOut(int ms = 1000);

    //returns true if SOME music is playing.
    bool playing();

    //So we can call if(music). Music converts to bool and is false whenever mSound==nullptr. 
    explicit operator bool();
};
