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

    bool mouseCapturedByGUI;

    int mousex,mousey;
    bool wdown, adown, sdown, ddown;

    bool mousefocus;
    bool keyboardfocus;


    bool quit; //false is keep running, true is "quit asap".

    Globals() : scWidth(640),scHeight(480),window(nullptr),renderer(nullptr),font16(nullptr),font28(nullptr),font16bold(nullptr),defaultClickSound(),mouseCapturedByGUI(false),mousex(0),mousey(0),wdown(false),adown(false),sdown(false),ddown(false),mousefocus(false),keyboardfocus(false),quit(false) {}
};

extern Globals g;