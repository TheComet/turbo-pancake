#pragma once
#include "UI.h"

class GSOptions
{
    Texture background;

    Button back;
    Button placeholder1;
    Button placeholder2;

    //Variables used in a transition/fade out animation
    //state 0 is fading in, state 1 is steady displaying, state 2 is fading out, state 3 is invisible.
    int transitionstate;
    Uint32 transitionstarttime;
    const Uint32 transitionduration=1000; //1 second transitions

    int stateChange; //0 if no state change should occur, 1 if we're changing to arena, 2 if options, 3 if noot.

public:
    GSOptions();
    void initialize();

    int getStateChange();
    void resetStateChange();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);
    
    bool doneTransitioning();

    //Start the fading out transition: transitionstate=2.
    void fadeOut();

    //Start the fading in transition: transitionstate=0.
    void fadeIn();

    //window resized event callback
    void windowResized();
};

