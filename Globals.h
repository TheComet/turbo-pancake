#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include "Sound.h"

//Just so we can make the globals super explicit (and call them as g.renderer, g.font, etc.) 
struct Globals {
	int scWidth, scHeight;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font16;
    TTF_Font *font28;
    TTF_Font *font16bold;
    Sound defaultClickSound;

    int mousex,mousey;
    bool wdown, adown, sdown, ddown;

    bool mousefocus;
    bool keyboardfocus;


    bool quit; //false is keep running, true is "quit asap".

    Globals() : scWidth(640),scHeight(480),window(NULL),renderer(NULL),font16(NULL),font28(NULL),quit(false) {}
};

extern Globals g;