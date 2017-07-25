#pragma once
#include "UI.h"
#include "ArenaMap.h"
#include "Camera.h"

class GSMapEditor
{
    Button back;
    Texture toggleBackground; 
    std::vector<ToggleButton> togglers;

    int editorx,editory;
    int editorpaint;

    void updateEditorPaint();

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu    

    bool leftclickheld;
    bool rightclickheld;
    int xtilelast;
    int ytilelast;
public:
    ArenaMap map;
    Camera cam;
    DraggingCameraController camController;

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