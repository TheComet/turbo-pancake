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
		else
			delist[0]->render();
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

void CharMan::render() {
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->render();
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
	
	switch (e->type) {
		case(SDL_KEYDOWN):
			switch (e->key.keysym.sym) {
				case(SDLK_w):
					if (spress)
						yacc = 0;
					else
						yacc = -acceleration;
					wpress = true;
					break;
				case(SDLK_a):
					if (dpress)
						xacc = 0;
					else
						xacc = -acceleration;
					apress = true;
					break;
				case(SDLK_s):
					if (wpress)
						yacc = 0;
					else
						yacc = acceleration;
					spress = true;
					break;
				case(SDLK_d):
					if (apress)
						xacc = 0;
					else
						xacc = acceleration;
					dpress = true;
					break;
			}
			break;
		case(SDL_KEYUP):
			switch (e->key.keysym.sym) {
				case(SDLK_w):
					yacc = 0;
					wpress = false;
					break;
				case(SDLK_a):
					xacc = 0;
					apress = false;
					break;
				case(SDLK_s):
					yacc = 0;
					spress = false;
					break;
				case(SDLK_d):
					xacc = 0;
					dpress = false;
					break;
			}
			break;
	} 

}

void TestCharacter::render(){
	t.render(xpos,ypos);
}
