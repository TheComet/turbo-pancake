#pragma once
#include "SDL.h"
#include "Button.h"
class GSArena
{
    Button back;

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu
public:
    GSArena();
    void initialize();

    int getStateChange();
    void resetStateChange();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);
};

