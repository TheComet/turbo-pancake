#pragma once
#include "Globals.h"
#include <vector>
#include "Texture.h"



/*
Game Manager class. Purpose is to handle switching between game states 

Game Manager should handle all the transitions between game states (changing menus, animating between menus,
changing from menu to actual gameplay, etc.)

*/
class GameMan {
    int state; // 0 for main menu, 1 for falling text
    

    Texture pressedbutton;
    Texture unpressedbutton;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<Texture> texts;

    void drawButtonUnpressed();
    void drawButtonPressed();

    bool mouseIsOverButton();


public:
    GameMan();
    //Textures have destructors already, so we shouldn't have to handle a dtor.

    //Load & initialize internal state.
    void initialize();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);
};