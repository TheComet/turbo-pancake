#pragma once
#include <vector>
#include "Texture.h"
#include "Camera.h"
enum Tiles {
    empty,
    wall,
    dirt
};
struct MapTile {
public:
    bool spawnable;
    Tiles ground;
    Tiles wall;
    int groundtexture;
    int walltexture; 
};

class ArenaMap {
    std::vector<Texture> tiletextures;
    int ntiles;
    std::vector<std::vector<int> > tiles;
    std::vector<std::vector<int> > walls;
    std::vector<std::vector<int> > walltextures;

    //clears and resizes the tiles/walls/walltextures vectors
    void resizeTileArrays();

    //given the "tiles" and "walls" array, recalculates "walltextures" to display the correct texture.
    void recalculateWallTiles();
public:
    ArenaMap();

    //initializes to a default map.
    void initialize();

    //Loads a circle map
    void loadEmptyMap(int circleradius=50);

    //true if 0<=x<ntiles && 0<=y<ntiles
    bool isInBounds(int x,int y);

    //draw the map according to the Camera's position and orientation.
    void draw(double x0,double y0,double width,Camera& cam);

    //returns this->ntiles.
    int getNTiles();
};