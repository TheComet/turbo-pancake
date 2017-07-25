#include "Character.h"
#include "GSArena.h"


/*
class AngleEaser {
    double target;
    Vector2 point;
    Vector2 easeexp(double dt) {
        point=(point-target)*exp(-rate*dt)+target;
        return point;
    }
    Vector2 easelinear(double dt) {
        double c=cos(angle);
        double s=sin(angle);
        double ctarget=cos(targetAngle);
        double starget=sin(targetAngle);
        double c2=cos(angle+dt*lookSpeed);
        double s2=sin(angle+dt*lookSpeed);
        double d1=(c-ctarget)*(c-ctarget)+(s-starget)*(s-starget);
        double d2=(c2-ctarget)*(c2-ctarget)+(s2-starget)*(s2-starget);
        if (d1<0.1)
            angle=targetAngle;
        else if (d2<d1)
            angle+=dt*lookSpeed;
        else
            angle-=dt*lookSpeed;
        double d=(point-target).length();
        if (d<rate*dt)
            point=target;
        else
            point=point+(target-point)*rate*dt/d;
        return point;
    }
};*/



Character::Character() : isdead(false),shoulddelete(false),
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
        Vector2 fpos=gs->cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        lookAt(fpos.x,fpos.y,false);
    }
}
void Character::handleEvent(SDL_Event *e,GSArena *gs) {
    if (e->type==SDL_KEYDOWN || e->type==SDL_KEYUP) {
        playerIsMovingLeft=g.adown;
        playerIsMovingRight=g.ddown;
        playerIsMovingUp=g.wdown;
        playerIsMovingDown=g.sdown;
    }
    else if (e->type==SDL_MOUSEMOTION) {
        Vector2 fpos=gs->cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        lookAt(fpos.x,fpos.y);
    }
    if (!g.mouseCapturedByGUI) {
        if (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_LEFT) {
            attack();
        }
        else if (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_RIGHT) {
            block();
        }
        else if (e->type==SDL_MOUSEBUTTONUP && e->button.button==SDL_BUTTON_LEFT) {

        }
        else if (e->type==SDL_MOUSEBUTTONUP && e->button.button==SDL_BUTTON_RIGHT) {
            idle();
        }
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
void TestCharacter::lookAt(double x,double y,bool showIcon) {
    targetAngle=atan2(y-pos.y,x-pos.x);
    if(showIcon)
        lastLookAt=SDL_GetTicks();
}

TestCharacter* TestCharacter::unsafe_copy() const{
    return new TestCharacter(*this);
}

TestCharacter::TestCharacter(double x,double y, float velcap, float acceleration, Texture img, Sound death) : Character(), deathSound(death), acc(),accMagnitude(acceleration),vel(),pos(x,y),speedcap(velcap),t(img),controlled(false),
  angle(0),targetAngle(0),lookSpeed(20),lastLookAt(0),directionIcon() ,sword(),shield(),attackmode(AttackMode::idlestate),
  attackStarted(0),attackDuration(0.15),shieldpos(Vector2(x,y),Vector2(x,y),40), swordpos(Vector2(x,y),Vector2(x,y),40) {
    directionIcon=loadTexture("media/character-pointing.png");
    sword=loadTexture("media/basic_sword.png");
    shield=loadTexture("media/basic_shield.png");
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


    //Super brute force "look at" method.
    //Calculates the distance between (cos(angle),sin(angle)) and (cos(targetAngle),sin(targetAngle)). 
    //if increasing angle decreases this distance, increase angle. Else decrease angle. And if 
    //they're close enough, just set angle=targetAngle.
    double c=cos(angle);
    double s=sin(angle);
    double ctarget=cos(targetAngle);
    double starget=sin(targetAngle);
    double c2=cos(angle+dt*lookSpeed);
    double s2=sin(angle+dt*lookSpeed);
    double d1=(c-ctarget)*(c-ctarget)+(s-starget)*(s-starget);
    double d2=(c2-ctarget)*(c2-ctarget)+(s2-starget)*(s2-starget);
    if (d1<0.1)
        angle=targetAngle;
    else if (d2<d1)
        angle+=dt*lookSpeed;
    else
        angle-=dt*lookSpeed;
    angle=fmod(angle,2*M_PI);

    if (attackmode==AttackMode::attackstate) {
        if (SDL_GetTicks()-attackStarted>(Uint32)(attackDuration*1000)) {
            attackmode=AttackMode::idlestate;
        }
    }

    swordpos.easeexp(dt);
    shieldpos.easeexp(dt);
}
void TestCharacter::attack() {
    if (attackmode==AttackMode::idlestate) {
        attackmode=AttackMode::attackstate;
        attackStarted=SDL_GetTicks();
    }

}
void TestCharacter::block() {
    if (attackmode==AttackMode::idlestate)
        attackmode=AttackMode::blockstate;

}
void TestCharacter::idle() {
    if (attackmode==AttackMode::blockstate)
        attackmode=AttackMode::idlestate;

}


void TestCharacter::render(const Camera& arg){
    arg.renderTexture(t,pos.x,pos.y,0,1);

    double timeSinceLook=(SDL_GetTicks()-lastLookAt)*0.001;
    double opacity=0.3;
    double startfade=0.2;
    double fadelength=0.4;
    if (timeSinceLook>startfade+fadelength) {
        opacity=0;
    }
    else if (timeSinceLook>startfade) {
        opacity=(1-(timeSinceLook-startfade)/fadelength)*0.3;
    }
    directionIcon.setAlpha((Uint8)(255*opacity));
    double iconRadius=0.6;
    arg.renderTexture(directionIcon,pos.x,pos.y,180.0*angle/M_PI,2);

    if (attackmode==AttackMode::attackstate) {
        double shieldoffset=60.0*M_PI/180.0;
        double shieldradius=0.55;
        double c1=cos(angle+shieldoffset);
        double s1=sin(angle+shieldoffset);
        //arg.renderTexture(shield,pos.x+c1*shieldradius,pos.y+s1*shieldradius,180.0*(angle+shieldoffset)/M_PI,0.6);
        arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldoffset)/M_PI,0.6);
        shieldpos.target=Vector2(c1,s1)*shieldradius;

        double Deltat=(SDL_GetTicks()-attackStarted)*0.001;
        double swordoffset=60.0*M_PI/180.0 - Deltat/attackDuration*120*M_PI/180.0;
        double swordradius=1;
        double c2=cos(angle-swordoffset);
        double s2=sin(angle-swordoffset);
        //arg.renderTexture(sword,pos.x+c2*swordradius,pos.y+s2*swordradius,30+90+180.0*(angle)/M_PI,1);
        arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,90+180.0*(angle-swordoffset)/M_PI,1);
        swordpos.target=Vector2(c2,s2)*swordradius;
    }
    else if (attackmode==AttackMode::blockstate) {
        double shieldoffset=0;
        double shieldradius=0.6;
        double c1=cos(angle+shieldoffset);
        double s1=sin(angle+shieldoffset);
        //arg.renderTexture(shield,pos.x+c1*shieldradius,pos.y+s1*shieldradius,180.0*(angle+shieldoffset)/M_PI,0.6);
        arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldoffset)/M_PI,0.6);
        shieldpos.target=Vector2(c1,s1)*shieldradius;

        double swordoffset=110.0*M_PI/180.0;
        double swordradius=0.45;
        double c2=cos(angle-swordoffset);
        double s2=sin(angle-swordoffset);
        //arg.renderTexture(sword,pos.x+c2*swordradius,pos.y+s2*swordradius,30+90+180.0*(angle)/M_PI,1);
        arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,30+90+180.0*(angle)/M_PI-180,1);
        swordpos.target=Vector2(c2,s2)*swordradius;

    }
    else if (attackmode==AttackMode::idlestate) {
        double shieldoffset=60.0*M_PI/180.0;
        double shieldradius=0.6;
        double c1=cos(angle+shieldoffset);
        double s1=sin(angle+shieldoffset);
        //arg.renderTexture(shield,pos.x+c1*shieldradius,pos.y+s1*shieldradius,180.0*(angle+shieldoffset)/M_PI,0.6);
        arg.renderTexture(shield,pos.x+shieldpos.point.x,pos.y+shieldpos.point.y,180.0*(angle+shieldoffset)/M_PI,0.6);
        shieldpos.target=Vector2(c1,s1)*shieldradius;

        double swordoffset=60.0*M_PI/180.0;
        double swordradius=0.6;
        double c2=cos(angle-swordoffset);
        double s2=sin(angle-swordoffset);
        //arg.renderTexture(sword,pos.x+c2*swordradius,pos.y+s2*swordradius,30+90+180.0*(angle)/M_PI,1);
        arg.renderTexture(sword,pos.x+swordpos.point.x,pos.y+swordpos.point.y,30+90+180.0*(angle)/M_PI,1);
        swordpos.target=Vector2(c2,s2)*swordradius;
    }
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