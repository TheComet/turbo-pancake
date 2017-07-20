#include "Character.h"


Character::Character() : acc(),accMagnitude(0),vel(),pos(),speedcap(0),t(),controlled(false) { }
Character::~Character() { }



TestCharacter::TestCharacter(int x, int y, float velcap, float acc, Texture img, Sound death) : Character() {
    pos=Vector2(x,y);
	speedcap = velcap;
	accMagnitude = acc;
	t = img;
	deathSound = death;
}

void TestCharacter::timestep(double dt, GSArena *gs) {
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
    double friction=1;
    vel=vel*fmax(1-friction*dt,0);

    //after final adjustments, move the character's coordinates
    pos+=vel*dt;
}

void TestCharacter::handleEvent(SDL_Event *e, GSArena *gs) {
    //Overkill to do this on every event. Maybe move to timestep? IDK, but it works!
    acc=Vector2();
    if (g.wdown)
        acc.y-=accMagnitude;
    if (g.adown)
        acc.x-=accMagnitude;
    if (g.sdown)
        acc.y+=accMagnitude;
    if (g.ddown)
        acc.x+=accMagnitude;
}

void TestCharacter::render(const Camera& arg){
    arg.renderTexture(t,pos.x,pos.y,0,1);
}
