#pragma once
#include "GSArena.h"

/*
class Geometry {

};
class CollisionHandler {
	std::vector<Geometry *> list;
public:
	getCollision()


};
*/

class Character {
	/*Geometry hitbox[];
	Behavior*/
	//int hp
public:


	virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs)=0;
	virtual void handleEvent(SDL_Event *e)=0;
	virtual void render()=0;
	/*
	virtual void moveLeft();
	virtual void moveRight();
	virtual void moveUp();
	virtual void moveDown();
	bool characterIsControlled();
	void toggleControl(); 

	*/
	


};

class TestCharacter {
	Texture t;
public:
	void timestep(double dt, GSArena *gs);
	void handleEvent(SDL_Event *e, GSArena *gs);
	void render(Camera *c);
};

class Camera {
	int x;
	int y;
	int zoom;

public:
	Camera();

	//Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
	void renderTexture(Texture &arg, double wx, double wy, double angle, double width);
};


