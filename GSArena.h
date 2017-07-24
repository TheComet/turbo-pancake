#pragma once
#include "SDL.h"
#include "Texture.h"
#include "UI.h"
#include <vector>
#include "Camera.h"
#include "Character.h"
#include "ArenaMap.h"


class GSArena;

class CharMan {
    //list of Characters. These could be active, playing death animations, or player controlled.
    CharacterList list;

    //Index of the currently controlled character. As above, if currentlyControlled=4 and we call removeChar(3), 
    //currentlyControlled is updated to 3 after the character is removed.
    int currentlyControlled;

    //remove the character at the given array index.
    void removeChar(int toRemove);

    //removes everything in indicesToDelete.
    void handleDeletions();

    //getters:
    size_t size() const;//returns list.size();

    //update list[currentlyControlled] to say it isn't character controlled and set 
    //currentlyControlled to -1.
    void revokeCharacterControl();
public:
    //Do nothing constructor
    CharMan();

    //Add a character. If assignControl is true, control is switched to this new character. 
    void addChar(const Character& chr, bool assignControl=false);

    //core game logic:
    void timestep(double dt,GSArena *const gs);
    void handleEvent(SDL_Event *e,GSArena *const gs);
    void render(Camera& arg);

    void killActiveCharacter();

    //Switches control to the next Character in charMan.
    //Returns true if you switched, false if there was no switch.
    bool switchControl();

    //returns true in a game over scenario: like if all the characters are dead.
    bool isGameOver();
};

/* ------------------------------------------------ TODO particle management TODO ---------------------------------------------------------------------- */

class Particle {
protected:
	//when this reaches lifetime, particle terminates
	float elapsed;
	//in sec
	float lifetime;
	Vector2 pos;
	float size; //in terms of width of tile
	float xvel;
	float yvel;
	bool canRemove;
public:
	Particle(Vector2 position, float s, float xv, float yv, float life = 1) : pos(position), size(s), xvel(xv),yvel(yv),lifetime(life), elapsed(0), canRemove(false) {}
	void timestep(double dt, GSArena *gs) {
		//for now lets just add to elapsed time
		elapsed += dt;
		pos.x += xvel * dt;
		pos.y += yvel * dt;
		if (elapsed >= lifetime) {
			canRemove = true;
		}
	}
	virtual void render(const Camera& arg)=0;
	bool shouldDelete() {
		return canRemove;
	}
};

class TextureParticle : public Particle {
	//efficiency purposes, lets only load the texture once
	Texture* img;
public:
	TextureParticle(Vector2 position, float siz, float xvel, float yvel, float life, Texture *t) : Particle(position, siz, xvel, yvel, life) {
		img = t;
	}
	void render(const Camera& arg) override {
		arg.renderTexture(*img,pos.x,pos.y,0,size);
	}
};

//crappy for now, just proof of implementation
class ParticleList {

	std::vector<std::vector<Particle*> > list;
	std::vector<Texture*> freelist;
	std::vector<Particle *> delist;

public:
	//empty constructor
	ParticleList() {}
	~ParticleList() {
		for (auto it = freelist.begin(); it != freelist.end(); it++) {
			delete *it;
		}
	}
	void addBurst(Vector2 pos, float s, int numParticles, int maxspeed, float life, std::string txtpath) {
		std::vector<Particle *> toadd;
		Texture * t = new Texture();
		t->load(txtpath);
		freelist.push_back(t);
		for (int i = 0; i < numParticles; i++) {
			float xv = (rand() / (float)RAND_MAX * maxspeed) - maxspeed/2;
			float yv = (rand() / (float)RAND_MAX * maxspeed) - maxspeed/2;
			toadd.push_back(new TextureParticle(pos, s, xv, yv, life, t));
		}
		list.push_back(toadd);
	}
	void timestep(double dt, GSArena *gs) {
		for (auto it = list.begin(); it != list.end(); it++) {
			for (auto jt = it->begin(); jt != it->end(); jt++) {
				(*jt)->timestep(dt, gs);
				if ((*jt)->shouldDelete()) {
					delist.push_back(*jt);
				}
			}
		}
		delist.clear();
	}

	void render(const Camera &arg) {
		for (auto it = list.begin(); it != list.end(); it++) {
			for (auto jt = it->begin(); jt != it->end(); jt++) {
				(*jt)->render(arg);
			}
		}
	}
	
};


class ParMan {
	//list of Characters. These could be active, playing death animations, or player controlled.
	ParticleList list;

	//remove the character at the given array index.
	void removeParticle(int toRemove);

	//removes everything in indicesToDelete.
	void handleDeletions();

	//getters:
	size_t size() const;//returns list.size();

public:
	//Do nothing constructor
	ParMan();

	//Add a character. If assignControl is true, control is switched to this new character. 
	void addParticle(const Particle& chr);

	//core game logic:
	void timestep(double dt, GSArena *const gs);
	void handleEvent(SDL_Event *e, GSArena *const gs);
	void render(Camera& arg);
};


/* ------------------------------------------------------- END particle management END -------------------------------------------------------------- */

class GSArena
{
    Button back;
    Button resetButton;

	bool gameOver;

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu    

public:
    ArenaMap map;
    Camera cam;
    CharMan charman;

    GSArena();
    void initialize();
    void reset();

    int getStateChange();
    void resetStateChange();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);

    //window resized event callback
    void windowResized();
};

