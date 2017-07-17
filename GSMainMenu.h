#pragma once
#include "Button.h"
class GSMainMenu
{
    //Main menu options
    Button arena;
    Button options;
    Button noot;
    Button otherbut;

    Texture backgroundimage;

    //Variables used in a transition/fade out animation
    //state 0 is fading in, state 1 is steady displaying, state 2 is fading out, state 3 is invisible.
    int transitionstate;
    Uint32 transitionstarttime;
    const int transitionduration=1000; //1 second transitions

    int stateChange; //0 if no state change should occur, 1 if we're changing to arena, 2 if options, 3 if noot.

public:
    GSMainMenu();

    //returns stateChange
    int getStateChange();

    void initialize();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);
};

