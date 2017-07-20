#pragma once
#include "SDL.h"
#include "Texture.h"
#include "UI.h"
#include <vector>
#include "Camera.h"
#include "Character.h"


class GSArena;

class CharMan {
    std::vector<Character *> list;
    std::vector<Character *> delist;
    Character *currentlyControlled;
public:
    CharMan() : currentlyControlled(NULL) {}
    CharMan(const CharMan& other) {}
    ~CharMan();
    void addChar(int x = 0,int y = 0,float velcap = 0,float acc = 0,Texture img = Texture(),Sound death = Sound(),bool assignControl = false);
    void removeChar(Character *toRemove = NULL);
    void timestep(double dt,GSArena *gs);
    void handleEvent(SDL_Event *e,GSArena *gs);
    void toggleControl();
    void toDelete();
    bool gameOver();
    void render(const Camera& arg);
};

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
    void draw(double x0, double y0, double width, Camera& cam);
    int getNTiles();
};
class GSArena
{
    Button back;
	bool gameOver;

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu    

public:
    ArenaMap map;
    Camera cam;
    CharMan charman;

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

