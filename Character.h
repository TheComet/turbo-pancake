#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "Sound.h"
#include <vector>



class GSArena;

class Character {
    virtual Character* unsafe_copy() const =0;
protected: 

    bool isdead; // true if the character is dead and should no longer be controlled by the player
    bool shoulddelete; //true if the character is done drawing and doing w/e and should be removed/deallocated from gameplay.
    bool playercontrolled; //true if the caller is passing player control on to the character.


    bool playerIsMovingUp;
    bool playerIsMovingDown;
    bool playerIsMovingRight;
    bool playerIsMovingLeft;

    struct InvulnerableList {
        Character *c;
        double t;
    };

    std::vector<InvulnerableList> invulnerableList;

    double health;

public:

    Character();

	//virtual Geometry *getGeometry()=0;
	virtual void timestep(double dt,GSArena *gs);
	virtual void handleEvent(SDL_Event *e, GSArena *gs); //default implementation passes stuff to move.
	virtual void render(const Camera& arg)=0;

    //Attack/whatever code.
    virtual bool dealDamage(double arg,Character *damageDealer,double invulnerabletime=0.15);

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

    virtual bool isPushable();

    virtual Vector2 getPos()=0;
    virtual void setPos(Vector2 pos)=0;
    virtual double getCharacterRadius()=0;

    //So that CharacterList has access to unsafe_copy
    friend class CharacterList;
};


class TestCharacter : public Character {
    //so many variables that it's important to organize them correctly!

    // ======================= Textures ======================= 
    Texture character;
    Texture sword;
    Texture shield;
    //Icon to display when the character looks somewhere else.
    Texture directionIcon;

    // ======================= Sounds ======================= 
    Sound deathSound;

    // ======================= Movement ======================= 
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    double accMagnitude;
    double speedcap; // velocity cap
    double characterRadius;

    // ======================= Angle ======================= 
    //Angle in radians
    double angle;
    //Angle to look towards
    double targetAngle;
    //Angle to look towards
    double lookSpeed;
    //time since the character looked somewhere else.
    Uint32 lastLookAt;

    // ======================= Attack ======================= 
    enum AttackMode {
        idlestate,
        attackstate,
        blockstate
    }  attackmode;
    Uint32 attackStarted;
    double attackDuration;
    VectorEaser shieldpos;
    VectorEaser swordpos;
    double damage;

    TestCharacter* unsafe_copy() const override;
public:
	TestCharacter(double x=0,double y=0, float velcap=0, float acc=0, Texture img=Texture(), Sound death=Sound());
    Vector2 getPos() override;
    void setPos(Vector2 pos) override;
    bool isPushable() override;
    double getCharacterRadius() override;

    void timestep(double dt, GSArena *gs) override;
	void render(const Camera& arg) override;


    //move in the positive x direction
    void moveRight() override; 
    //move in the positive x direction
    void moveLeft() override; 
    //move in the NEGATIVE y direction (screen space up!)
    void moveUp() override; 
    //move in the positive y direction
    void moveDown() override; 
    //move in a certain direction (magnitude is ignored)
    void moveDir(double x,double y) override; 
    //look at the world coordinate x,y.
    void lookAt(double x,double y,bool showIcon=true) override; 
    void attack() override;
    void block() override;
    void idle() override;
    void kill() override;
    bool dealDamage(double arg,Character *damageDealer,double invulnerabletime=0.15);

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