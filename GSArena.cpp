#include "GSArena.h"
#include "Character.h"

using namespace std;

CharMan::CharMan() : list(), currentlyControlled(-1) {}


void CharMan::removeChar(int toRemove) {
    Character &c=list[toRemove];
    if (c.isPlayerControlled()) {
        revokeCharacterControl();
    }
    else if (toRemove<currentlyControlled) {
        currentlyControlled--;
    }
    list.erase(list.begin()+toRemove);
}
void CharMan::handleDeletions() {
    for (int i=0;i<(int)list.size();i++) {
        if (list[i].shouldDelete()){
            std::cout<<"Removing character "<<i<<". currentlyControlled is "<<currentlyControlled<<". list.size()="<<list.size()<<std::endl;

            removeChar(i);

            std::cout<<"Removed character "<<i<<". currentlyControlled is "<<currentlyControlled<<". list.size()="<<list.size()<<std::endl;
            i--;
            
        }
    }
}
size_t CharMan::size() const {
    return list.size();
}
void CharMan::revokeCharacterControl() {
    if (currentlyControlled>-1) {
        Character &c2=list[currentlyControlled];
        if (!c2.isPlayerControlled())
            cout<<"Error in CharMan::revokeCharacterControl! A Character thinks it's currently controlled but CharMan says it isn't!"<<endl;
        c2.setPlayerControlled(false);
        currentlyControlled=-1;
    }
}

void CharMan::addChar(const Character &arg, bool assignControl) {
    list.push_back(arg);
    Character &c=list[list.size()-1];
    c.setPlayerControlled(assignControl);
    if (assignControl) {
        revokeCharacterControl();
        currentlyControlled = (int)list.size()-1;
    }
}

void CharMan::timestep(double dt,GSArena *const gs) {
    for (auto it = list.begin(); it != list.end(); it++) {
        (*it)->timestep(dt,gs);
    }
    handleDeletions();
}

void CharMan::handleEvent(SDL_Event *e,GSArena *const gs) {
    //only take keyboard input for current controllable unit
    if (currentlyControlled>=0)
        list[currentlyControlled].handleEvent(e,gs);
}

void CharMan::render(Camera& cam) {
    for (auto it = list.begin(); it != list.end(); it++) {
        (*it)->render(cam);
    }
}

bool CharMan::switchControl() {
    std::cout<<"Switching Control"<<std::endl;
    //check to make sure valid controllable unit present
    if (list.size() == 0)
        return false;

    //Some code to find the next available non-dead
    int newcontrol=(currentlyControlled+1)%list.size();
    int firstIndex=newcontrol;
    bool firststep=true;
    while ((list[newcontrol].isDead()||list[newcontrol].shouldDelete()) && (newcontrol!=firstIndex || firststep)) {
        newcontrol=(newcontrol+1)%list.size();
        firststep=false;
    }
    std::cout<<"newcontrol="<<newcontrol<<". firstindex="<<firstIndex<<". firststep="<<firststep<<std::endl;
    if (newcontrol==currentlyControlled)
        return false;
    //else we've found a valid, not dead, not "shouldDelete"able character to control.

    revokeCharacterControl();
    currentlyControlled=newcontrol;
    list[currentlyControlled].setPlayerControlled(true);
    return true;
}
bool CharMan::isGameOver() {
    return !(list.size() > 0);
}
void CharMan::killActiveCharacter() {
    if (currentlyControlled<0)
        return;
    list[currentlyControlled].kill();
}

/*
std::vector<Texture> tiletextures;
int ntiles;
std::vector<std::vector<int> > tiles;*/
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
    loadEmptyMap(10);

}
void ArenaMap::resizeTileArrays() {
    tiles=vector<vector<int> >(ntiles, (vector<int>(ntiles,-1)));
    walls=vector<vector<int> >(ntiles,(vector<int>(ntiles,-1)));
    walltextures=vector<vector<int> >(ntiles,(vector<int>(ntiles,-1)));
}
int ArenaMap::getNTiles() {
    return ntiles;
}
void ArenaMap::loadEmptyMap(int circleradius) {

    /* 
    Make a circle-based tile map and add walls around it.
    There's some super ugly code for walking around the circle. Read at your own peril.
    */

    if (circleradius<5)
        circleradius=5;
    ntiles=2*circleradius+1;
    resizeTileArrays();
    for (int i=0;i<ntiles;i++)
        for (int j=0;j<ntiles;j++)
            if (double(i-circleradius-0.5)*(i-circleradius-0.5)+double(j-circleradius-0.5)*(j-circleradius-0.5) < double(circleradius-2)*(circleradius-2))
                tiles[i][j]=0;
            else
                tiles[i][j]=-1;

    int lastdx,lastdy,initialx,initialy,x,y,dx,dy;

    //Set x to be the first empty tile, on the middle rightmost part of the map, that butts up against the circle.
    x=ntiles-1; 
    y=circleradius;
    while (x>0 && tiles[x][y]==-1) {
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
        if (tiles[x-dy][y+dx]!=0) {
            //then we're turning left.
            newdx=-dy; newdy=dx;
        } else if(tiles[x+dx][y+dy]==0) {
            newdx=dy; newdy=-dx;
        }
        else {

            newdx=dx;
            newdy=dy;
        }
        dx=newdx;dy=newdy;

        walls[x][y]=1;
        walltextures[x][y]=1;

        x+=dx;
        y+=dy;
        
        lastdx=dx;
        lastdy=dy;
    }

    for (int i=0;i<ntiles;i++) {
        for (int j=0;j<ntiles;j++) {
            tiles[i][j]=0;
        }
    }
    recalculateWallTiles();
}
bool ArenaMap::isInBounds(int x,int y) {
    return (x>0)&&(y>0)&&(x<ntiles)&&(y<ntiles);
}
void ArenaMap::recalculateWallTiles() {
    /* 
    walls[x][y] is a list of 0's or 1's depending on if there is or isn't a wall.
    We want to find what texture to display, based on which walls are adjacent.
    Shitty problem in tile based texturing.
    But it finds the correct texture and places it in walltexture[x][y].
    */
    for (int x=0;x<ntiles;x++) {
        for (int y=0;y<ntiles;y++) {
            int t=walls[x][y];
            if (t<0)
                walltextures[x][y]=-1;
            else {
                int a=0;
                int b=0;
                int c=0;
                int d=0;
                if (isInBounds(x+1,y) && t==walls[x+1][y]) {
                    a=1;
                }
                if (isInBounds(x,y+1) && t==walls[x][y+1]) {
                    b=1;
                }
                if (isInBounds(x-1,y) && t==walls[x-1][y]) {
                    c=1;
                }
                if (isInBounds(x,y-1) && t==walls[x][y-1]) {
                    d=1;
                }
                int k=a|(d<<1)|(c<<2)|(b<<3);
                walltextures[x][y]=1+k;
            }
        }
    }
}

void ArenaMap::draw(double x0,double y0,double width, Camera& cam) {
    for (int i=0;i<ntiles;i++) {
        for (int j=0;j<ntiles;j++) {
            int x,y,wx,wy;

            if (tiles[i][j]>=0) {
                Texture &t=tiletextures[tiles[i][j]];
                cam.renderTile(t,i,j,1);
            }
            if (walltextures[i][j]>=0) {
                Texture &t=tiletextures[walltextures[i][j]];
                cam.renderTile(t,i,j,1);
            }
        }
    }
}

GSArena::GSArena() : back(),stateChange(0),map(),gameOver(false),cam() {
    cam.setDraggable();
}

//returns stateChange
int GSArena::getStateChange() {
    return stateChange;
}
void GSArena::resetStateChange() {
    stateChange=0;
}

void GSArena::initialize() {
    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/backbutton_pressed.png");
    Texture up=loadTexture("media/backbutton.png");

    back=Button(g.scWidth -10 - p.getWidth(),10,p,up);

    p=loadTexture("media/buttonLong_blue_pressed.png");
    up=loadTexture("media/buttonLong_blue.png");
    resetButton=Button(10,g.scHeight-10-p.getHeight(),p,up,"Reset Chars");

    stateChange=0;
    map.initialize();

    //test character
    //xcoord, ycoord, vel cap, acc
    Texture charactertexture=loadTexture("media/character.png"); //only load texture once for four characters.
    charman.addChar(TestCharacter(3,3,3,3,charactertexture,Sound()),true);
    charman.addChar(TestCharacter(3,10,3,3,charactertexture,Sound()));
    charman.addChar(TestCharacter(10,3,3,3,charactertexture,Sound()));
    charman.addChar(TestCharacter(10,10,3,3,charactertexture,Sound()));
}

void GSArena::reset() {
    charman=CharMan();
    Texture charactertexture=loadTexture("media/character.png"); //only load texture once for four characters.
    charman.addChar(TestCharacter(3,3,3,3,charactertexture,Sound()),true);
    charman.addChar(TestCharacter(3,10,3,3,charactertexture,Sound()));
    charman.addChar(TestCharacter(10,3,3,3,charactertexture,Sound()));
    charman.addChar(TestCharacter(10,10,3,3,charactertexture,Sound()));
}

//Timestep. Calls timestep on the current active game state.
void GSArena::timestep(double dt) {
    if (back.isPressed()) {
        stateChange=1;
        back.pressReceived();
    }
    if (resetButton.isPressed()) {
        reset();
        resetButton.pressReceived();
    }
    cam.timestep(dt);
    charman.timestep(dt,this);
}
void GSArena::render() {
    map.draw(map.getNTiles()/2,map.getNTiles()/2,50, cam);
    charman.render(cam);
    back.render();
    resetButton.render();
}
void GSArena::handleEvent(SDL_Event *e) {
	if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_PERIOD)
        charman.switchControl();
	else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_k) {
        charman.killActiveCharacter();
        charman.switchControl();
		if (charman.isGameOver()) {
			gameOver = true;
			//do gameOver rendering stuff somewhere TODO
		}
	}

    if (!g.mouseCapturedByGUI) {
        g.mouseCapturedByGUI=g.mouseCapturedByGUI||back.handleEvent(e);
        g.mouseCapturedByGUI=g.mouseCapturedByGUI||resetButton.handleEvent(e);
        if(!g.mouseCapturedByGUI)
            g.mouseCapturedByGUI=g.mouseCapturedByGUI||cam.handleEvent(e);
    }
	
    charman.handleEvent(e, this);
}

//window resized event callback
void GSArena::windowResized() {
    back.setPos(g.scWidth -10 - back.getUnpressedTexture().getWidth(),10);
}