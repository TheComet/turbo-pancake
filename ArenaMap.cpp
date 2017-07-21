#include "ArenaMap.h"
using namespace std;


ArenaMap::ArenaMap() : tiletextures(),ntiles(0),tiles() { }
void ArenaMap::initialize() {
    tiletextures.push_back(loadTexture("media/tile.png"));
    tiletextures.push_back(loadTexture("media/0000wall.png"));
    tiletextures.push_back(loadTexture("media/0001wall.png"));
    tiletextures.push_back(loadTexture("media/0010wall.png"));
    tiletextures.push_back(loadTexture("media/0011wall.png"));
    tiletextures.push_back(loadTexture("media/0100wall.png"));
    tiletextures.push_back(loadTexture("media/0101wall.png"));
    tiletextures.push_back(loadTexture("media/0110wall.png"));
    tiletextures.push_back(loadTexture("media/0111wall.png"));
    tiletextures.push_back(loadTexture("media/1000wall.png"));
    tiletextures.push_back(loadTexture("media/1001wall.png"));
    tiletextures.push_back(loadTexture("media/1010wall.png"));
    tiletextures.push_back(loadTexture("media/1011wall.png"));
    tiletextures.push_back(loadTexture("media/1100wall.png"));
    tiletextures.push_back(loadTexture("media/1101wall.png"));
    tiletextures.push_back(loadTexture("media/1110wall.png"));
    tiletextures.push_back(loadTexture("media/1111wall.png"));
    createCircleMap(13);
}
void ArenaMap::resetMap() {
    tiles=vector<vector<MapTile> >(ntiles,(vector<MapTile>(ntiles,MapTile())));
}
int ArenaMap::getNTiles() {
    return ntiles;
}
void ArenaMap::createCircleMap(int circleradius) {

    /*
    Make a circle-based tile map and add walls around it.
    There's some super ugly code for walking around the circle. Read at your own peril.
    */

    if (circleradius<5)
        circleradius=5;
    ntiles=2*circleradius+1;
    resetMap();
    for (int i=0;i<ntiles;i++)
        for (int j=0;j<ntiles;j++)
            if (double(i-circleradius-0.5)*(i-circleradius-0.5)+double(j-circleradius-0.5)*(j-circleradius-0.5) < double(circleradius-2)*(circleradius-2)) {
                tiles[i][j]=MapTile(Tiles::dirt,Tiles::empty,-1);
                if (i<=circleradius/2)
                    tiles[i][j].spawnteam=0;
                else if (i>((circleradius*3)/2))
                    tiles[i][j].spawnteam=1;
            }
            else
                tiles[i][j]=MapTile(Tiles::empty,Tiles::empty);;

    int lastdx,lastdy,initialx,initialy,x,y,dx,dy;

    //Set x to be the first empty tile, on the middle rightmost part of the map, that butts up against the circle.
    x=ntiles-1;
    y=circleradius;
    while (x>0 && tiles[x][y].ground==Tiles::empty) {
        x--;
    }
    x++;

    lastdx=0;
    lastdy=1;

    initialx=x,initialy=x;
    bool firststep=true;
    dx=0;
    dy=1;

    int stepcutoff=1000;
    int steps=0;
    while (x>0 && y>0 && x<ntiles && y<ntiles && (firststep || (x!=initialx || y!=initialy)) && steps<stepcutoff) {
        steps++;
        firststep=false;

        int newdx,newdy;
        if (tiles[x-dy][y+dx].ground!=Tiles::dirt) {
            //then we're turning left.
            newdx=-dy; newdy=dx;
        }
        else if (tiles[x+dx][y+dy].ground==Tiles::dirt) {
            newdx=dy; newdy=-dx;
        }
        else {

            newdx=dx;
            newdy=dy;
        }
        dx=newdx;dy=newdy;

        tiles[x][y].wall=Tiles::wall;

        x+=dx;
        y+=dy;

        lastdx=dx;
        lastdy=dy;

    }

    for (int i=0;i<ntiles;i++) {
        for (int j=0;j<ntiles;j++) {
            tiles[i][j].ground=Tiles::dirt;
            tiles[i][j].groundtexture=0;
        }
    }
    recalculateTextures();

    addHouseShape(circleradius-3,circleradius-3,7,7);
    addHouseShape(circleradius-2,circleradius-2-4,5,5);
    addHouseShape(circleradius-2,circleradius-2+4,5,5);
}
bool ArenaMap::isInBounds(int x,int y) {
    return (x>0)&&(y>0)&&(x<ntiles)&&(y<ntiles);
}

//Loads a square with holes into it into the map.
void ArenaMap::addHouseShape(int x,int y,int w,int h) {
    for (int i=0;i<w;i++) {
        if (i!=(w/2)) {
            if(isInBounds(x+i,y))
                tiles[x+i][y].wall=Tiles::wall;
            if (isInBounds(x+i,y+h-1))
                tiles[x+i][y+h-1].wall=Tiles::wall;
        }
        else {
            if (isInBounds(x+i,y))
                tiles[x+i][y].wall=Tiles::empty;
            if (isInBounds(x+i,y+h-1))
                tiles[x+i][y+h-1].wall=Tiles::empty;
        }
    }
    for (int i=0;i<h;i++) {
        if (i!=(h/2)) {
            if(isInBounds(x,y+i))
                tiles[x][y+i].wall=Tiles::wall;
            if (isInBounds(x+w-1,y+i))
                tiles[x+w-1][y+i].wall=Tiles::wall;
        }
        else {
            if (isInBounds(x,y+i))
                tiles[x][y+i].wall=Tiles::empty;
            if (isInBounds(x+w-1,y+i))
                tiles[x+w-1][y+i].wall=Tiles::empty;
        }
    }
    recalculateTextures();
}

//puts a wall at x,y and updates the map.
void ArenaMap::addWall(int x,int y) {
    tiles[x][y].wall=Tiles::wall;
    recalculateTextures();
}

//returns all tiles that have the spawnable flag set to true.
std::vector<IntegerPoint> ArenaMap::getSpawnablePoints(int team) {
    std::vector<IntegerPoint> ret;
    for (int x=0;x<ntiles;x++) {
        for (int y=0;y<ntiles;y++) {
            if (tiles[x][y].spawnteam==team)
                ret.push_back(IntegerPoint(x,y));
        }
    }
    return ret;
}
void ArenaMap::recalculateTextures() {
    /*
    walls[x][y] is a list of 0's or 1's depending on if there is or isn't a wall.
    We want to find what texture to display, based on which walls are adjacent.
    Shitty problem in tile based texturing.
    But it finds the correct texture and places it in walltexture[x][y].
    */
    for (int x=0;x<ntiles;x++) {
        for (int y=0;y<ntiles;y++) {
            Tiles t=tiles[x][y].wall;
            if (t==Tiles::empty)
                tiles[x][y].walltexture=-1;
            else {
                int a=0;
                int b=0;
                int c=0;
                int d=0;
                if (isInBounds(x+1,y) && t==tiles[x+1][y].wall) {
                    a=1;
                }
                if (isInBounds(x,y+1) && t==tiles[x][y+1].wall) {
                    b=1;
                }
                if (isInBounds(x-1,y) && t==tiles[x-1][y].wall) {
                    c=1;
                }
                if (isInBounds(x,y-1) && t==tiles[x][y-1].wall) {
                    d=1;
                }
                int k=a|(d<<1)|(c<<2)|(b<<3);
                tiles[x][y].walltexture=1+k;
            }
        }
    }
}

void ArenaMap::render(Camera& cam) {
    for (int i=0;i<ntiles;i++) {
        for (int j=0;j<ntiles;j++) {
            if (tiles[i][j].groundtexture>=0) {
                Texture &t=tiletextures[tiles[i][j].groundtexture];
                cam.renderTile(t,i,j,1);
            }
            if (tiles[i][j].walltexture>=0) {
                Texture &t=tiletextures[tiles[i][j].walltexture];
                cam.renderTile(t,i,j,1);
            }
        }
    }
}