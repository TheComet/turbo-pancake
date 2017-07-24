#pragma once
#include <vector>
#include "Texture.h"
#include "Camera.h"
#include "CollisionUtil.h"
enum Tiles {
    empty,
    wall,
    dirt
};
struct IntegerPoint {
    int x; int y;
    IntegerPoint() : x(0),y(0) { }
    IntegerPoint(int a,int b) : x(a),y(b) { }
};
struct MapTile {
public:
    Tiles ground;
    Tiles wall;
    int spawnteam;
    int groundtexture;
    int walltexture;
    MapTile() : ground(Tiles::dirt),wall(Tiles::empty),spawnteam(-1),groundtexture(0),walltexture(0) { }
    MapTile(Tiles uground, Tiles uwall,int uspawnteam=-1, int ugroundtexture=-1, int uwalltexture=-1) : ground(uground),wall(uwall),spawnteam(uspawnteam),groundtexture(ugroundtexture),walltexture(uwalltexture) { }
};

class ArenaMap {
    std::vector<Texture> tiletextures;
    int ntiles;
    std::vector<std::vector<MapTile> > tiles;
    std::vector<std::vector<std::vector<Rectangle> > > collisionGeometry;

    //clears and resizes the tiles/walls/walltextures vectors to nsize
    void resetMap();

    void recalculateTextures();

    //Loads a circle map
    void createCircleMap(int circleradius=50);

    //Loads a square with holes into it into the map.
    void addHouseShape(int x=0,int y=0, int w=3, int h=3);

    Texture team1spawn;
    Texture team2spawn;
public:

    void circleMapCollide(Vector2 &c,double r);
    //precondition: recalculateTextures has already been called.
    void recalculateCollisionGeometry();

    ArenaMap();

    //puts a wall at x,y and updates the map.
    void addWall(int x,int y);
    void eraseWall(int x,int y);

    //returns all tiles that have the spawnable flag set to true.
    std::vector<IntegerPoint> getSpawnablePoints(int team);

    //initializes to a default map.
    void initialize();


    //true if 0<=x<ntiles && 0<=y<ntiles
    bool isInBounds(int x,int y);

    //draw the map according to the Camera's position and orientation.
    void render(Camera& cam);
    void renderSpawns(Camera& cam);
    void setTile(int x,int y,Tiles tiletype);
    void setSpawn(int x,int y,int team);

    //returns this->ntiles.
    int getNTiles();
};