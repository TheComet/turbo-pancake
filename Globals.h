#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

//Just so we can make the globals super explicit (and call them as g.renderer, g.font, etc.) 
struct Globals {
	int scWidth, scHeight;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font16;
    TTF_Font *font28;

    bool quit; //false is keep running, true is "quit asap".

    Globals() : scWidth(640),scHeight(480),window(NULL),renderer(NULL),font16(NULL),font28(NULL),quit(false) {}
};

extern Globals g;