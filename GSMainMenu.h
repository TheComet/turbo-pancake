#pragma once
#include "Button.h"

class GSMainMenu
{
    //Main menu options
    Button arena;
    Button options;
    Button noot;

    Music menumusic;

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
    void resetStateChange();
    bool doneTransitioning();

    void initialize();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);

    //Start the fading out transition: transitionstate=2.
    void fadeOut();

    //Start the fading in transition: transitionstate=0.
    void fadeIn();

    //window resized event callback
    void windowResized();
};

