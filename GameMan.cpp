#include "GameMan.h"
#include <iostream>


GameMan::GameMan() : gsmain(),gsnoot(),state(gsEnum::mainmenu) {}

//Load & initialize internal state.
void GameMan::initialize() {
    gsmain.initialize();
    gsnoot.initialize();

}

//Timestep. Calls timestep on the current active game state.
void GameMan::timestep(double dt) {
    if (state==gsEnum::mainmenu) {
        gsmain.timestep(dt);
    } else if (state==gsEnum::menutonoot) {
        gsmain.timestep(dt);
        gsnoot.timestep(dt);
    } else if (state==gsEnum::noot) {
        gsnoot.timestep(dt);
    } else if (state==gsEnum::noottomenu) {
        gsmain.timestep(dt);
        gsnoot.timestep(dt);
    }
}

void GameMan::render() {
    if (state==gsEnum::mainmenu) {
        gsmain.render();
    }
    else if (state==gsEnum::menutonoot) {
        gsnoot.render(); // order is important: main will render on top of noot.
        gsmain.render();
    }
    else if (state==gsEnum::noot) {
        gsnoot.render();
    }
    else if (state==gsEnum::noottomenu) {
        gsnoot.render();
        gsmain.render();
    }
}
void GameMan::handleEvent(SDL_Event *e) {
    if (state==gsEnum::mainmenu) {
        gsmain.handleEvent(e);
    }
    else if (state==gsEnum::menutonoot) {
        gsnoot.handleEvent(e); // order is important: main will render on top of noot.
        gsmain.handleEvent(e);
    }
    else if (state==gsEnum::noot) {
        gsnoot.handleEvent(e);
    }
    else if (state==gsEnum::noottomenu) {
        gsnoot.handleEvent(e);
        gsmain.handleEvent(e);
    }
}