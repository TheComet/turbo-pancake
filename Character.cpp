#include "Character.h"

TestCharacter::TestCharacter(int x, int y, float velcap, float acc, Texture img) : Character() {
	xpos = x;
	ypos = y;
	speedcap = velcap;
	acceleration = acc;
	t = img;
	wpress = apress = spress = dpress = false;
}

void TestCharacter::timestep(double dt, GSArena *gs) {
	yvelocity += yacc * dt;
	xvelocity += xacc * dt;
	
	float friction = g.friction * dt;
	//smooth stopping, apply global friction
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


Camera::Camera() {

}

//Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
void Camera::renderTexture(Texture &arg, double wx, double wy, double angle, double width) {

}