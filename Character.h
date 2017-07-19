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
	bool wpress, apress, spress, dpress;
public:

	Character() : xacc(0), yacc(0), xvelocity(0), yvelocity(0) {}

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs)=0;
	virtual void handleEvent(SDL_Event *e, GSArena *gs)=0;
	virtual void render()=0;
	
	/*void moveLeft(float speed);
	void moveRight(float speed);
	void moveUp(float speed);
	void moveDown(float speed);
	bool characterIsControlled();
	void toggleControl(); 

	*/
	


};

class TestCharacter : Character {

public:
	TestCharacter(int x=0, int y=0, float velcap=0, float acc=0, Texture img=Texture());
	~TestCharacter() {}
	void timestep(double dt, GSArena *gs);
	void handleEvent(SDL_Event *e, GSArena *gs);
	void render();
};

