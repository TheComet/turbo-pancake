#include "GSArena.h"
#include "Character.h"
#include "Particles.h"
#include "ArenaMap.h"

using namespace std;

CharMan::CharMan() : list(), currentlyControlled(-1) {}

//list of particle effects
ParticleList plist;

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
            //std::cout<<"Removing character "<<i<<". currentlyControlled is "<<currentlyControlled<<". list.size()="<<list.size()<<std::endl;

            removeChar(i);

            //std::cout<<"Removed character "<<i<<". currentlyControlled is "<<currentlyControlled<<". list.size()="<<list.size()<<std::endl;
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

void CharMan::handlePushableCollision() {
    std::vector<Character*> pushables;
    for (auto it = list.begin(); it != list.end(); it++) {
        if ((*it)->isPushable()) {
            pushables.push_back(*it);
        }
    }
    for (size_t i=0;i<pushables.size();i++) {
        for (size_t j=i+1;j<pushables.size();j++) {
            Vector2 ipos=pushables[i]->getPos();
            double ir=pushables[i]->getCharacterRadius();
            Vector2 jpos=pushables[j]->getPos();
            double jr=pushables[j]->getCharacterRadius();
            circleCirclePush(ipos,ir,jpos,jr);
            pushables[i]->setPos(ipos);
            pushables[j]->setPos(jpos);
        }
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
    handlePushableCollision();
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
    //std::cout<<"Switching Control"<<std::endl;
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
    //std::cout<<"newcontrol="<<newcontrol<<". firstindex="<<firstIndex<<". firststep="<<firststep<<std::endl;
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

Character *CharMan::getActiveCharacter() {
    if (currentlyControlled<0)
        return nullptr;
    return &list[currentlyControlled];
}

GSArena::GSArena() : back(),stateChange(0),map(),gameOver(false),cam(),camController() {
    
}

//returns stateChange
int GSArena::getStateChange() {
    return stateChange;
}
void GSArena::resetStateChange() {
    stateChange=0;
}

void GSArena::initialize() {
    stickers=Stickers();
    camController.attachCamera(&cam);
    //camController.setDraggable();
    //camController.setDragButton(2);
    camController.centerTarget();

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
    reset();
}

void GSArena::reset() {
    stickers=Stickers();
    charman=CharMan();
    std::vector<IntegerPoint> sp1=map.getSpawnablePoints(0);
    std::vector<IntegerPoint> sp2=map.getSpawnablePoints(1);
    int a=rand()%sp1.size();
    int b=rand()%sp1.size();
    int c=rand()%sp2.size();
    int d=rand()%sp2.size();

    //Loading the texture four times creates four separate textures that can be colored
    //separately. 
    charman.addChar(TestCharacter(sp1[a].x,sp1[a].y,4,150,loadTexture("media/character.png"),Sound()),true);
    charman.addChar(TestCharacter(sp1[b].x,sp1[b].y,4,150,loadTexture("media/character.png"),Sound()));
    charman.addChar(TestCharacter(sp2[c].x,sp2[c].y,4,150,loadTexture("media/character.png"),Sound()));
    charman.addChar(TestCharacter(sp2[d].x,sp2[d].y,4,150,loadTexture("media/character.png"),Sound()));
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

    Character *target=charman.getActiveCharacter();
    if(target!=nullptr)
        camController.setTargetPosition(target->getPos());

    camController.timestep(dt);
    charman.timestep(dt,this);
	plist.timestep(dt, this);
}
void GSArena::render() {
    map.render(cam);
    stickers.render(cam);
    charman.render(cam);

    back.render();
    resetButton.render();
	plist.render(cam);
}
void GSArena::handleEvent(SDL_Event *e) {
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_PERIOD) {
        charman.switchControl();
        camController.centerTarget();
    }
	else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_k) {
        charman.killActiveCharacter();
		//test directional burst
        Character *ch=charman.getActiveCharacter();
        if (ch!=nullptr) {
            std::cout << "charman: " << ch->getPos() << std::endl;
            plist.addDirectionalBurst(ch->getPos(),90,15,0.35f,20,10,2,"media/team2spawn.png",.05f);
        }
		charman.switchControl();
        camController.centerTarget();
		if (charman.isGameOver()) {
			gameOver = true;
			//do gameOver rendering stuff somewhere TODO
		}
	}
	else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p) {

		plist.addBurst(Vector2(5,10), 1, 20, 10, 2, "media/team2spawn.png", .1f);
	}

    g.mouseCapturedByGUI=g.mouseCapturedByGUI||back.handleEvent(e);
    g.mouseCapturedByGUI=g.mouseCapturedByGUI||resetButton.handleEvent(e);
    if (!g.mouseCapturedByGUI) 
        camController.handleEvent(e);
    
	
    charman.handleEvent(e, this);
}

//window resized event callback
void GSArena::windowResized() {
    back.setPos(g.scWidth -10 - back.getUnpressedTexture().getWidth(),10);
}