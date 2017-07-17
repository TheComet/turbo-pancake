#pragma once
#include "SDL.h"
#include "Texture.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Button.h"

class GSNoot
{
    Button back;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<Texture> texts;

    int stateChange; //0 is not transitioning, 1 is going to main menu.
public:
    GSNoot();

    //returns stateChange
    int getStateChange();

    void initialize();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);
};

