#include "Character.h"

CharMan::~CharMan() {
	for (auto it = list.begin(); it != list.end(); it++) {
		delete *it;
	}
	list.clear();
}

void CharMan::addChar(int x, int y, float velcap, float acc, Texture img, Sound death, bool assignControl) {
	/*Sound *deathSound = new Sound();
	deathSound.load(death); */
	Character *newChar = (Character *)new TestCharacter(x, y, velcap, acc, img, death);
	list.push_back(newChar);
	if (assignControl)
		currentlyControlled = newChar;
}

void CharMan::removeChar(Character * toRemove) {
	//right now we can just take no arg and kill controlled unit
	if (!toRemove)
		toRemove = currentlyControlled;
	for (auto it = list.begin(); it != list.end(); it++) {
		if (*it == toRemove) {
			//check if we just killed controlled unit.. if so toggle
			if (toRemove == currentlyControlled)
				toggleControl();

			//was that the only character? if so, set ptr to null
			//check if we just killed controlled unit.. if so toggle
			if (toRemove == currentlyControlled)
				currentlyControlled = NULL;

			//death sound and then delete
			(*it)->deathSound.play();
			delist.push_back(*it);
			list.erase(it);
			break;
		}
	}
}

/*
*	Performs final death sounds + animations(later on) before removing from map.
*/
void CharMan::toDelete() {
	if (delist.size() > 0) {
		if (!(delist[0]->deathSound.playing())) {
			std::cout << "this ran" << std::endl;
			delete delist[0];
			delist.erase(delist.begin());
		}
	}
}

void CharMan::timestep(double dt, GSArena *gs) {
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->timestep(dt, gs);
	}
}

void CharMan::handleEvent(SDL_Event *e, GSArena *gs) {
	//only take keyboard input for current controllable unit
	if (currentlyControlled)
		currentlyControlled->handleEvent(e, gs);
}

void CharMan::render(const Camera& cam) {
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->render(cam);
	}
	toDelete();
}

void CharMan::toggleControl() {
	//check to make sure valid controllable unit present
	if (list.size() == 0)
		return;
	//just toggle to next available in list
	for (auto it = list.begin(); it != list.end(); it++) {
		if (*it == currentlyControlled) {
			if (++it == list.end())
				currentlyControlled = *(list.begin());
			else
				currentlyControlled = *it;
			break;
		}
	}
}
bool CharMan::gameOver() {
    return !(list.size() > 0);
}

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
