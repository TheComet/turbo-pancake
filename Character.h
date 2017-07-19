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
protected:
	float xacc;
	float yacc;
	float acceleration; //acceleration
	float xvelocity;
	float yvelocity;
	float xpos;
	float ypos;
	float speedcap; // velocity cap
	Texture t;
	bool wpress, apress, spress, dpress; //for opposite key press checking
	bool controlled;
public:

	Character() : xacc(0), yacc(0), xvelocity(0), yvelocity(0), controlled(false) {}
	virtual ~Character() {}

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs)=0;
	virtual void handleEvent(SDL_Event *e, GSArena *gs)=0;
	virtual void render()=0;
	Sound deathSound;
	/*void moveLeft(float speed);
	void moveRight(float speed);
	void moveUp(float speed);
	void moveDown(float speed);
	bool characterIsControlled(); */
	/*void toggleControl() {
		controlled ^= controlled;
	}

	bool beingControlled() {
		return controlled;
	} */

};

class TestCharacter : Character {

public:
	TestCharacter(int x=0, int y=0, float velcap=0, float acc=0, Texture img=Texture(), Sound death=Sound());
	~TestCharacter() {}
	void timestep(double dt, GSArena *gs);
	void handleEvent(SDL_Event *e, GSArena *gs);
	void render();
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

class CharMan {
	std::vector<Character *> list;
	std::vector<Character *> delist;
	Character *currentlyControlled;
public:
	CharMan() : currentlyControlled(NULL) {}
	CharMan(const CharMan& other) {}
	~CharMan();
	void addChar(int x = 0, int y = 0, float velcap = 0, float acc = 0, Texture img = Texture(), Sound death = Sound(), bool assignControl = false);
	void removeChar(Character *toRemove = NULL);
	void timestep(double dt, GSArena *gs);
	void handleEvent(SDL_Event *e, GSArena *gs);
	void toggleControl();
	void toDelete();
	bool gameOver() {
		return !(list.size() > 0);
	}
	void render();
};
