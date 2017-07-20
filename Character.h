#pragma once
#include "GSArena.h"
#include "Camera.h"
#include "Vector2.h"


class Character {
protected:
    Vector2 acc;
    double accMagnitude;
    Vector2 vel;
    Vector2 pos;
	double speedcap; // velocity cap

	Texture t;

	bool controlled;
public:

    Character();
    virtual ~Character();

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs)=0;
	virtual void handleEvent(SDL_Event *e, GSArena *gs)=0;
	virtual void render(const Camera& arg)=0;
	Sound deathSound;
};

class TestCharacter : Character {

public:
	TestCharacter(int x=0, int y=0, float velcap=0, float acc=0, Texture img=Texture(), Sound death=Sound());
	~TestCharacter() {}
	void timestep(double dt, GSArena *gs);
	void handleEvent(SDL_Event *e, GSArena *gs);
	void render(const Camera& arg);
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
    bool gameOver();
	void render(const Camera& arg);
};
