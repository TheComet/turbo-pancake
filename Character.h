#pragma once
#include "Camera.h"
#include "Vector2.h"
#include <vector>

class VectorEaser {

public:
    Vector2 point;
    Vector2 target;
    double rate;
    VectorEaser() : point(),target(),rate(0) { }
    VectorEaser(Vector2 upoint,Vector2 utarget,double urate) : point(upoint),target(utarget),rate(urate) { }
    Vector2 easeexp(double dt) {
        point=(point-target)*exp(-rate*dt)+target;
        return point;
    }
    Vector2 easelinear(double dt) {
        double d=(point-target).length();
        if (d<rate*dt)
            point=target;
        else
            point=point+(target-point)*rate*dt/d;
        return point;
    }
};

class GSArena;

class Character {

protected: 

    virtual Character* unsafe_copy() const =0;

    bool isdead; // true if the character is dead and should no longer be controlled by the player
    bool shoulddelete; //true if the character is done drawing and doing w/e and should be removed/deallocated from gameplay.
    bool playercontrolled; //true if the caller is passing player control on to the character.


    bool playerIsMovingUp;
    bool playerIsMovingDown;
    bool playerIsMovingRight;
    bool playerIsMovingLeft;
public:

    Character();

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs);
	virtual void handleEvent(SDL_Event *e, GSArena *gs); //default implementation passes stuff to move.
	virtual void render(const Camera& arg)=0;

    //The idea with this movement interface is that you should be able to control
    //any type of creature! Maybe as an easter egg or something. So that if we make a 
    //Mouse : public Character or a GiantGoliath : public Character, these can be 
    //made player controllable with 0 extra coding effort!
    virtual void moveRight()=0; //move in the positive x direction
    virtual void moveLeft()=0; //move in the positive x direction
    virtual void moveUp()=0; //move in the NEGATIVE y direction (screen space up!)
    virtual void moveDown()=0; //move in the positive y direction
    virtual void moveDir(double x, double y)=0; //move in a certain direction (magnitude is ignored)
    virtual void lookAt(double x,double y,bool showIcon=true)=0; //look at the world coordinate x,y.
    virtual void attack()=0; 
    virtual void block()=0;
    virtual void idle()=0;

    //getters and setters for determining if a character is dead/ready to be deleted/player controlled.
    //an "isDead" character is not controllable, but may be playing some animation or doing some important work,
    //so it can't be deleted yet. A "shouldDelete" character can be removed from memory. A isPlayerControlled
    //character should have information passed to its event handler.
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
    Sound deathSound;
    Vector2 acc;
    double accMagnitude;
    Vector2 vel;
    Vector2 pos;
    double speedcap; // velocity cap

    Texture t;
    bool controlled;

    TestCharacter* unsafe_copy() const override;

    //Angle in radians
    double angle;
    //Angle to look towards
    double targetAngle;
    //Angle to look towards
    double lookSpeed;
    //time since the character looked somewhere else.
    Uint32 lastLookAt;
    //Icon to display when the character looks somewhere else.
    Texture directionIcon;

    Texture sword;
    Texture shield;
    enum AttackMode {
        idlestate,
        attackstate,
        blockstate
    };

    AttackMode attackmode;
    Uint32 attackStarted;
    double attackDuration;

    VectorEaser shieldpos;
    VectorEaser swordpos;
    


public:
	TestCharacter(double x=0,double y=0, float velcap=0, float acc=0, Texture img=Texture(), Sound death=Sound());
	~TestCharacter() {}

    void timestep(double dt, GSArena *gs) override;
	//void handleEvent(SDL_Event *e, GSArena *gs) override;
	void render(const Camera& arg) override;

    void moveRight(); //move in the positive x direction
    void moveLeft(); //move in the positive x direction
    void moveUp(); //move in the NEGATIVE y direction (screen space up!)
    void moveDown(); //move in the positive y direction
    void moveDir(double x,double y); //move in a certain direction (magnitude is ignored)
    void lookAt(double x,double y,bool showIcon=true); //look at the world coordinate x,y.
    void attack();
    void block();
    void idle();

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