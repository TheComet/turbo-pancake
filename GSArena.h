#pragma once
#include "SDL.h"
#include "Texture.h"
#include "UI.h"
#include <vector>
#include "Camera.h"
#include "Character.h"
#include "ArenaMap.h"
#include "Stickers.h"
#include "Particles.h"


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

    void handlePushableCollision();
public:
    //Do nothing constructor
    CharMan();

    //Add a character. If assignControl is true, control is switched to this new character. 
    void addChar(const Character& chr, bool assignControl=false);

    //calls timestep on all sub-units:
    void timestep(double dt,GSArena *const gs);
    //calls handleEvent on all sub-units:
    void handleEvent(SDL_Event *e,GSArena *const gs);
    //calls render on all sub-units:
    void render(Camera& arg);

    //returns the active character. Returns nullptr if there is no active character.
    Character *getActiveCharacter();

    void killActiveCharacter();

    //Switches control to the next Character in charMan.
    //Returns true if you switched, false if there was no switch.
    bool switchControl();

    //returns true in a game over scenario: like if all the characters are dead.
    bool isGameOver();

    CharacterList *getChars() {
        return &list;
    }
};


class GSArena
{
    Button back;
    Button resetButton;

	bool gameOver;

    int stateChange; //0 if no state change should occur, 1 if we're changing to main menu    

public:
    ArenaMap map;
    Camera cam;
    FollowerCameraController camController;
    CharMan charman;
    Stickers stickers;
    ParticleList plist;

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

