#pragma once
#include "SDL.h"
#include "Texture.h"
#include "UI.h"
#include <vector>

class ArenaMap {
    std::vector<Texture> tiletextures;
    int ntiles;
    std::vector<std::vector<int> > tiles;
    std::vector<std::vector<int> > walls;
    std::vector<std::vector<int> > walltextures;

public:
    ArenaMap();
    void initialize();
    void loadEmptyMap(int circleradius=50);
    void resizeTileArrays();
    void recalculateWallTiles();
    bool isInBounds(int x,int y);
    //
    void draw(double x0, double y0, double width);
    int getNTiles();
};
class GSArena
{
    Button back;
	bool gameOver;

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu    
    ArenaMap map;
public:
    GSArena();
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

