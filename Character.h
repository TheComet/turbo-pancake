#pragma once
#include "Camera.h"
#include "Vector2.h"
#include <vector>

class GSArena;

class Character {

protected: 

    virtual Character* unsafe_copy() const =0;

    bool isdead; // true if the character is dead and should no longer be controlled by the player
    bool shoulddelete; //true if the character is done drawing and doing w/e and should be removed/deallocated from gameplay.
    bool playercontrolled; //true if the caller is passing player control on to the character.

public:
    Sound deathSound;

    Character();

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs)=0;
	virtual void handleEvent(SDL_Event *e, GSArena *gs)=0;
	virtual void render(const Camera& arg)=0;

    //getters and setters for determining if a character is dead/ready to be deleted/player controlled.
    virtual bool isDead() const;
    virtual bool shouldDelete() const;
    virtual void setDead(bool arg);
    virtual void setDelete(bool arg);
    virtual bool isPlayerControlled() const;
    virtual void setPlayerControlled(bool arg);
    virtual void kill();


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

    TestCharacter* unsafe_copy() const override;

public:
	TestCharacter(double x=0,double y=0, float velcap=0, float acc=0, Texture img=Texture(), Sound death=Sound());
	~TestCharacter() {}
 

    void timestep(double dt, GSArena *gs) override;
	void handleEvent(SDL_Event *e, GSArena *gs) override;
	void render(const Camera& arg) override;

    //So that CharacterList has access to unsafe_copy
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
    void eraseNth(int n);
    void push_back(const Character& arg);
    void clear();

};