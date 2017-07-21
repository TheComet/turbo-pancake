#pragma once
#include "Camera.h"
#include "Vector2.h"
#include <vector>

class GSArena;

class Character {

protected: 

    virtual Character* unsafe_copy() const =0;
public:
    Sound deathSound;

    Character();

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs)=0;
	virtual void handleEvent(SDL_Event *e, GSArena *gs)=0;
	virtual void render(const Camera& arg)=0;

    //So that CharacterList has access to unsafe_copy
    friend class CharacterList;
};

class TestCharacter : public Character {
    Vector2 acc;
    double accMagnitude;
    Vector2 vel;
    Vector2 pos;
    double speedcap; // velocity cap

    Texture t;
    bool controlled;

    //So that CharacterList has access to unsafe_copy
    TestCharacter* unsafe_copy() const;

public:
	TestCharacter(double x=0,double y=0, float velcap=0, float acc=0, Texture img=Texture(), Sound death=Sound());
	~TestCharacter() {}
	void timestep(double dt, GSArena *gs);
	void handleEvent(SDL_Event *e, GSArena *gs);
	void render(const Camera& arg);

    friend class CharacterList;
};

class CharacterList {

    std::vector<Character *> list;

    //Deletes the memory and replaces list with an empty list.
    void free();

    //Returns a deep copy of list, by calling "new" a bunch of time, & Character's assignment operator.
    //Hands off the responsibility of deallocating memory to the caller.
    std::vector<Character *> unsafe_copy() const;
public:
    //empty constructor
    CharacterList();

    //Rule of 3
    CharacterList(const CharacterList &arg);
    CharacterList &operator=(const CharacterList &arg);
    ~CharacterList();

    typedef std::vector<Character*>::iterator iterator;

    //std::vector-like stuff
    //Accessors
    Character& at(int i);
    Character& operator[](int i);
    //because size_t is overrated
    int size() const;
    //iterator stuff
    iterator begin();
    iterator end();
    void erase(iterator pt);
    void push_back(const Character& arg);
};