#include "Character.h"
#include "GSArena.h"

Character::Character() : deathSound() ,isdead(false),shoulddelete(false),
playercontrolled(false),
playerIsMovingUp(false),
playerIsMovingDown(false),
playerIsMovingRight(false),
playerIsMovingLeft(false) { }

bool Character::isDead()  const { return isdead; }
bool Character::shouldDelete()  const { return shoulddelete;  }
void Character::setDead(bool arg) { isdead=arg;  }
void Character::setDelete(bool arg) { shoulddelete=arg; }
bool Character::isPlayerControlled() const { return playercontrolled; }
void Character::setPlayerControlled(bool arg) { playercontrolled=arg; }
void Character::kill() { setDead(true); setDelete(true); }

void Character::timestep(double dt,GSArena *gs) {
    if (isPlayerControlled()) {
        if (playerIsMovingUp)
            moveUp();
        if (playerIsMovingDown)
            moveDown();
        if(playerIsMovingRight)
            moveRight();
        if (playerIsMovingLeft)
            moveLeft();
    }
}
void Character::handleEvent(SDL_Event *e,GSArena *gs) {
    if (e->type==SDL_KEYDOWN || e->type==SDL_KEYUP) {
        playerIsMovingLeft=g.adown;
        playerIsMovingRight=g.ddown;
        playerIsMovingUp=g.wdown;
        playerIsMovingDown=g.sdown;
    }
}

void TestCharacter::moveRight() {
    acc.x+=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveLeft() {
    acc.x-=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveUp() {
    acc.y-=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveDown(){
    acc.y+=accMagnitude;
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::moveDir(double x,double y) {
    acc=Vector2(x,y);
    if (acc.length()>0)
        acc=accMagnitude*acc.normalized();
}
void TestCharacter::lookAt(double x,double y) {

}

TestCharacter* TestCharacter::unsafe_copy() const{
    return new TestCharacter(*this);
}

TestCharacter::TestCharacter(double x,double y, float velcap, float acceleration, Texture img, Sound death) : Character(), acc(),accMagnitude(acceleration),vel(),pos(x,y),speedcap(velcap),t(img),controlled(false)  {
	deathSound = death;
}

void TestCharacter::timestep(double dt, GSArena *gs) {
    Character::timestep(dt,gs);

    vel+=acc*dt;
    //0 the vector if it gets too small.
    if (vel.length()<0.0005)
        vel=Vector2();
    //If velocity is greater than speedcap, set it to length speedcap.
    if (vel.length()>speedcap)
        vel=vel.normalized()*speedcap;

    //Apply friction.
    //This corresponds to viscous friction x''(t)=-friction*x'(t) instead of constant friction,
    //but we can tweak it if we want.
    double friction=5;
    if(acc.length2()<0.001)
        vel=vel*fmax(1-friction*dt,0);

    //after final adjustments, move the character's coordinates
    pos+=vel*dt;
    gs->map.circleMapCollide(pos,0.47);
    acc=Vector2(); //the character does not keep accelerating unless moveLeft is constantly called.
}

void TestCharacter::handleEvent(SDL_Event *e, GSArena *gs) {
    Character::handleEvent(e,gs);
}

void TestCharacter::render(const Camera& arg){
    arg.renderTexture(t,pos.x-0.5,pos.y-0.5,0,1);
}


//Deletes the memory and replaces list with an empty list.
void CharacterList::free() {
    for (auto i=list.begin(); i!=list.end();i++) {
        delete *i;
    }
    list=std::vector<Character *>();
}

//Returns a deep copy of list, by calling "new" a bunch of time, & Character's unsafe_copy function.
//Hands off the responsibility of deallocating memory to the caller.
std::vector<Character *> CharacterList::unsafe_copy() const {
    std::vector<Character *> ret;
    for (auto i=list.begin(); i!=list.end();i++) {
        ret.push_back((*i)->unsafe_copy());
    }
    return ret;
}
//empty constructor
CharacterList::CharacterList() : list() { }

//Rule of 3
CharacterList::CharacterList(const CharacterList &arg) : list() {
    list=arg.unsafe_copy();
}
CharacterList& CharacterList::operator=(const CharacterList &arg) {
    //Because we're calling new and delete here to make a deep copy,
    //we do have to check for self-assignment.
    if (&arg==this)
        return *this;

    //If we haven't been assigned to ourself, we can copy and free.
    free();
    list=arg.unsafe_copy();
    return *this;
}
CharacterList::~CharacterList() {
    free();
}

//std::vector-like stuff
//Accessors
Character& CharacterList::at(int i) {
    return *list[i];
}
Character& CharacterList::operator[](int i) {
    return *list[i];
}
//because size_t is overrated
int CharacterList::size() const {
    return (int)list.size();
}
//iterator stuff
CharacterList::iterator CharacterList::begin() {
    return list.begin();
}
CharacterList::iterator CharacterList::end() {
    return list.end();

}
void CharacterList::erase(iterator pt) {
    delete *pt;
    list.erase(pt);
}
void CharacterList::eraseNth(int n) {
    CharacterList::iterator pt=list.begin()+n;
    delete *pt;
    list.erase(pt);
}
void CharacterList::push_back(const Character& arg) {
    list.push_back(arg.unsafe_copy());
}
void CharacterList::clear() {
    free();
}