#pragma once
#include <vector>
#include "Texture.h"
#include "Vector2.h"
#include "Camera.h"


class GSArena;

class Particle {
protected:
	//when this reaches lifetime, particle terminates
	double elapsed;
	//in sec
	double lifetime;
	Vector2 pos;
	double size; //in terms of width of tile
	double xvel;
	double yvel;
	bool canRemove;
	double delay; //if not all particles at once. in seconds

public:
	Particle(Vector2 position, double s, double xv, double yv, double life = 1, double delay = 0) : pos(position), size(s), xvel(xv), yvel(yv), lifetime(life), elapsed(0), canRemove(false), delay(delay) {}
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
	Texture img;
public:
	TextureParticle(Vector2 position, double siz, double xvel, double yvel, double life, double delay, Texture t) : Particle(position, siz, xvel, yvel, life, delay) {
		img = t;
	}
	void render(const Camera& arg) override {
		if (elapsed >= delay)
			arg.renderTexture(img, pos.x - 0.5, pos.y - 0.5, 0, size);
	}
};

//crappy for now, just proof of implementation
class ParticleList {

	std::vector<std::vector<Particle*> > list;
    void clearAllParticles() {
        
    }

    //Disable copy constructor and assignment.
    ParticleList(const ParticleList& srcMyClass);
    ParticleList& operator=(const ParticleList& srcMyClass);

public:
	//empty constructor
	ParticleList() {}
	~ParticleList() {
        for (auto a=list.begin();a!=list.end();a++)
            for (auto b=(*a).begin();b!=(*a).end();b++)
                if (*b !=nullptr)
                    delete *b;
        list.clear();
	}
	void addBurst(Vector2 pos, double s, int numParticles, int maxspeed, double life, std::string texpath, double spawndelay = 0) {
		std::vector<Particle *> toadd;
		Texture t = loadTexture(texpath);

		for (int i = 0; i < numParticles; i++) {
			double xv = (rand() / (double)RAND_MAX * 2 * maxspeed) - maxspeed;
			double yv = (rand() / (double)RAND_MAX * 2 * maxspeed) - maxspeed;
			toadd.push_back(new TextureParticle(pos, s, xv, yv, life, spawndelay*i, t));
		}

		list.push_back(toadd);
	}
	void addDirectionalBurst(Vector2 pos, double initangle, double incidentangle, double s, int numParticles, int maxspeed, double life, std::string texpath, double spawndelay = 0) {
		std::vector<Particle *> toadd;
        Texture t = loadTexture(texpath);

		for (int i = 0; i < numParticles; i++) {
			double angle = ((rand() / (double)RAND_MAX * 2 * incidentangle) - incidentangle + initangle) * M_PI / 180.0;
			double speed = rand() / (double)RAND_MAX * maxspeed;
			double xv = speed * cos(angle);
			double yv = speed * sin(-angle);
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