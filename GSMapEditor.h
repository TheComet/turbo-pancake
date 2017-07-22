#pragma once
#include "UI.h"
#include "ArenaMap.h"
#include "Camera.h"

class GSMapEditor
{
    Button back;

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu    

    bool leftclickheld;
    bool rightclickheld;
    int xtilelast=0;
    int ytilelast=0;
public:
    ArenaMap map;
    Camera cam;

    GSMapEditor();
    void initialize();

    int getStateChange();
    void resetStateChange();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);

    //window resized event callback
    void windowResized();
};