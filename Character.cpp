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

Character::Character() : xacc(0),yacc(0),xvelocity(0),yvelocity(0),controlled(false) { }
Character::~Character() { }



TestCharacter::TestCharacter(int x, int y, float velcap, float acc, Texture img, Sound death) : Character() {
	xpos = x;
	ypos = y;
	speedcap = velcap;
	acceleration = acc;
	t = img;
	deathSound = death;
	wpress = apress = spress = dpress = false;
}

void TestCharacter::timestep(double dt, GSArena *gs) {
	yvelocity += yacc * dt;
	xvelocity += xacc * dt;
	

	float friction = g.friction * dt;

	//smooth stopping, apply global friction (only if not running)
	if (yacc == 0) {
		if (yvelocity < 0) {
			if (yvelocity > -friction)
				yvelocity = 0;
			else
				yvelocity += friction;
		}
		else if (yvelocity > 0) {
			if (yvelocity < friction)
				yvelocity = 0;
			else
				yvelocity -= friction;
		}
	}
	if (xacc == 0) {
		if (xvelocity < 0) {
			if (xvelocity > friction)
				xvelocity = 0;
			else
				xvelocity += friction;
		}
		else if (xvelocity > 0) {
			if (xvelocity < friction)
				xvelocity = 0;
			else
				xvelocity -= friction;
		}
	}


	//make sure velocity doesnt exceed speed cap
	if (yvelocity > speedcap)
		yvelocity = speedcap;
	else if (yvelocity < -speedcap)
		yvelocity = -speedcap;
	if (xvelocity > speedcap)
		xvelocity = speedcap;
	else if (xvelocity < -speedcap)
		xvelocity = -speedcap;

	//after final adjustments, move the character's coordinates
	xpos = xpos + xvelocity * dt;
	ypos = ypos + yvelocity * dt;
}

void TestCharacter::handleEvent(SDL_Event *e, GSArena *gs) {
    xacc=yacc=0;
    if (g.wdown)
        yacc-=acceleration;
    if (g.adown)
        xacc-=acceleration;
    if (g.sdown)
        yacc+=acceleration;
    if (g.ddown)
        xacc+=acceleration;
}

void TestCharacter::render(const Camera& arg){
    arg.renderTexture(t,xpos,ypos,0,1);
    //t.render(xpos,ypos);
}
