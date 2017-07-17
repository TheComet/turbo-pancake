#pragma once
#include "Globals.h"
#include <vector>
#include "Texture.h"
#include "Button.h"
#include "GSMainMenu.h"
#include "GSNoot.h"

//flags to determine what state we're in and if there's any transition going on.
enum gsEnum {
    mainmenu, 
    menutonoot,
    noot,
    noottomenu
};

/*
Game Manager class. Purpose is to handle switching between game states 

Game Manager should handle all the transitions between game states (changing menus, animating between menus,
changing from menu to actual gameplay, etc.)

*/
class GameMan {
    GSMainMenu gsmain;
    GSNoot gsnoot; 
    
    gsEnum state;
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