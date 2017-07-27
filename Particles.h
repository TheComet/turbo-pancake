#pragma once
#include <vector>
#include "Texture.h"
#include "Vector2.h"
#include "Camera.h"
#include <string>


class GSArena;

class Particle {
protected:
	//when this reaches lifetime, particle terminates
	double elapsed;
	//in sec
	double lifetime;
	Vector2 pos;
	double size; //in terms of width of tile
    Vector2 vel;
	bool canRemove;
	double delay; //if not all particles at once. in seconds

public:
    Particle() : pos(),size(0),vel(),lifetime(0),elapsed(0),canRemove(false),delay(0) { }
	Particle(Vector2 position, double s, Vector2 v, double life = 1, double delay = 0) : pos(position), size(s), vel(v), lifetime(life), elapsed(0), canRemove(false), delay(delay) {}
	virtual void timestep(double dt, GSArena *gs) {
		//for now lets just add to elapsed time
		if (elapsed >= delay) {
			pos += vel * dt;
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
	TextureParticle(Vector2 position, double siz, Vector2 v, double life, double delay, Texture t) : Particle(position, siz, v, life, delay) {
		img = t;
	}
	void render(const Camera& arg) override {
		if (elapsed >= delay)
			arg.renderTexture(img, pos.x, pos.y, 0, size);
	}
};
class FloatingTextParticle : public Particle {
    //efficiency purposes, lets only load the texture once
    Texture img;
    double phase;
public:
    FloatingTextParticle(Vector2 position,double height, SDL_Color c,std::string str) :
        Particle(position,height,Vector2(0,0),3,0) {
        img = textTexture(str,c,g.font16bold);
        phase=2*M_PI*(rand()/(double)RAND_MAX);
    }
    virtual void timestep(double dt,GSArena *gs) {
        //for now lets just add to elapsed time
        
        vel+=Vector2(cos(phase+elapsed*2*M_PI)*1,-1)*dt;
        pos += vel * dt;
        if (elapsed >= lifetime + delay) {
            canRemove = true;
        }
        elapsed += dt;
    }
    void render(const Camera& arg) override {
        if (elapsed >= delay)
            arg.renderTextureHeight(img,pos.x,pos.y,0,size);
    }
};

//crappy for now, just proof of implementation
class ParticleList {

	std::vector<Particle*> list;

    //Disable copy constructor and assignment.
    ParticleList(const ParticleList& srcMyClass);
    ParticleList& operator=(const ParticleList& srcMyClass);
public:
	//empty constructor
	ParticleList() {}
	~ParticleList() {
        for (auto a=list.begin();a!=list.end();a++)
            if ((*a) !=nullptr)
                delete *a;
        list.clear();
	}
    void addTextParticle(Vector2 pos,double height,SDL_Color c,std::string str) {
        list.push_back(new FloatingTextParticle(pos,height,c,str));
    }
	void addBurst(Vector2 pos, double s, int numParticles, int maxspeed, double life, std::string texpath, double spawndelay = 0) {
		Texture t = loadTexture(texpath);

		for (int i = 0; i < numParticles; i++) {
			double xv = (rand() / (double)RAND_MAX * 2 * maxspeed) - maxspeed;
			double yv = (rand() / (double)RAND_MAX * 2 * maxspeed) - maxspeed;
			list.push_back(new TextureParticle(pos, s, Vector2(xv, yv), life, spawndelay*i, t));
		}
	}
	void addDirectionalBurst(Vector2 pos, double initangle, double incidentangle, double s, int numParticles, int maxspeed, double life, std::string texpath, double spawndelay = 0) {
        Texture t = loadTexture(texpath);

		for (int i = 0; i < numParticles; i++) {
			// ANGLE IS IN RADIANS!
			double angle = ((rand() / (double)RAND_MAX * 2 * incidentangle) - incidentangle + initangle);
			double speed = rand() / (double)RAND_MAX * maxspeed;
			double xv = speed * cos(angle);
			double yv = speed * sin(-angle);
			list.push_back(new TextureParticle(pos, s, Vector2(xv, yv), life, spawndelay*i, t));
		}
	}
	void timestep(double dt, GSArena *gs) {
		//okay, we're iterating through our list of particle lists!
		auto jt = list.begin();
		while (jt != list.end()) {
			//call timestep on each particle
			(*jt)->timestep(dt, gs);

			if ((*jt)->shouldDelete()) {
				//time to delete the particle. set iterator to position after erasing it from list.
				delete *jt;
				jt = list.erase(jt);
			}
			else {
				++jt;
			}
		}
	}

	void render(const Camera &arg) {
		for (auto it = list.begin(); it != list.end(); it++) {
			(*it)->render(arg);
		}
	}

};