#pragma once
#include <vector>
#include "Texture.h"
#include "Vector2.h"
#include "Camera.h"


class GSArena;

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
	float delay; //if not all particles at once. in ms

public:
	Particle(Vector2 position, float s, float xv, float yv, float life = 1, float delay = 0) : pos(position), size(s), xvel(xv), yvel(yv), lifetime(life), elapsed(0), canRemove(false), delay(delay) {}
	void timestep(double dt, GSArena *gs) {
		//for now lets just add to elapsed time
		if (elapsed >= delay) {
			pos.x += xvel * dt;
			pos.y += yvel * dt;
			if (elapsed >= lifetime + delay) {
				canRemove = true;
			}
		}
		elapsed += dt;
	}
	virtual void render(const Camera& arg) = 0;
	bool shouldDelete() {
		return canRemove;
	}
};

class TextureParticle : public Particle {
	//efficiency purposes, lets only load the texture once
	Texture* img;
public:
	TextureParticle(Vector2 position, float siz, float xvel, float yvel, float life, float delay, Texture *t) : Particle(position, siz, xvel, yvel, life, delay) {
		img = t;
	}
	void render(const Camera& arg) override {
		if (elapsed >= delay)
			arg.renderTexture(*img, pos.x - 0.5, pos.y - 0.5, 0, size);
	}
};

//crappy for now, just proof of implementation
class ParticleList {

	std::vector<std::vector<Particle*> > list;
	std::vector<Texture*> freelist;

public:
	//empty constructor
	ParticleList() {}
	~ParticleList() {
		for (auto it = freelist.begin(); it != freelist.end(); it++) {
			delete *it;
		}
	}
	void addBurst(Vector2 pos, float s, int numParticles, int maxspeed, float life, std::string txtpath, float spawndelay = 0) {
		std::vector<Particle *> toadd;
		Texture * t = new Texture();
		t->load(txtpath);
		freelist.push_back(t);

		for (int i = 0; i < numParticles; i++) {
			float xv = (rand() / (float)RAND_MAX * 2 * maxspeed) - maxspeed;
			float yv = (rand() / (float)RAND_MAX * 2 * maxspeed) - maxspeed;
			toadd.push_back(new TextureParticle(pos, s, xv, yv, life, spawndelay*i, t));
		}

		list.push_back(toadd);
	}
	void addDirectionalBurst(Vector2 pos, float initangle, float incidentangle, float s, int numParticles, int maxspeed, float life, std::string txtpath, float spawndelay = 0) {
		std::vector<Particle *> toadd;
		Texture * t = new Texture();
		t->load(txtpath);
		freelist.push_back(t);

		for (int i = 0; i < numParticles; i++) {
			double angle = ((rand() / (float)RAND_MAX * 2 * incidentangle) - incidentangle + initangle) * M_PI / 180.0;
			float speed = rand() / (float)RAND_MAX * maxspeed;
			float xv = speed * cos(angle);
			float yv = speed * sin(-angle);
			toadd.push_back(new TextureParticle(pos, s, xv, yv, life, spawndelay*i, t));
		}

		list.push_back(toadd);
	}
	void timestep(double dt, GSArena *gs) {
		//okay, we're iterating through our list of particle lists!
		auto it = list.begin();
		while (it != list.end()) {

			//remove empty particle lists (expired effect)
			if (it->empty()) {
				it = list.erase(it);
			}
			else {
				auto jt = it->begin();

				//iterating through particle list
				while (jt != it->end()) {
					//call timestep on each particle
					(*jt)->timestep(dt, gs);

					if ((*jt)->shouldDelete()) {
						//time to delete the particle. set iterator to position after erasing it from list.
						delete *jt;
						jt = it->erase(jt);
					}
					else {
						++jt;
					}
				}
				//move on to next effect
				++it;
			}
		}
	}

	void render(const Camera &arg) {
		for (auto it = list.begin(); it != list.end(); it++) {
			for (auto jt = it->begin(); jt != it->end(); jt++) {
				(*jt)->render(arg);
			}
		}
	}

};