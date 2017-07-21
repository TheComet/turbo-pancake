#include "GameMan.h"
#include <iostream>

GameMan::GameMan() : gsmain(),gsnoot(), gsoptions(),gsarena(),state(gsEnum::mainmenu),audio(10,10) {}

//Load & initialize internal state.
void GameMan::initialize() {
    gsmain.initialize();
    gsnoot.initialize();
    gsoptions.initialize();
    //gsarena.initialize();
}

//Timestep. Calls timestep on the current active game state.
void GameMan::timestep(double dt) {
    if (state==gsEnum::mainmenu) {
        gsmain.timestep(dt);
        int statechange=gsmain.getStateChange();
        if (statechange) {
            if (statechange==1) {//If GSMainMenu requests a state change to "arena", fade out & change our state flag accordingly.
                state=gsEnum::menutoarena;
                gsmain.fadeOut();
            } else if (statechange==2) { //If GSMainMenu requests a state change to "options", fade out & change our state flag accordingly.
                state=gsEnum::menutooptions;
                gsmain.fadeOut();
                gsoptions.fadeIn();
            } else if (statechange==3) { //If GSMainMenu requests a state change to "noot", fade out & change our state flag accordingly.
                state=gsEnum::menutonoot;
                gsmain.fadeOut();
            }
            //reset the stateChange variable so that if we wind back up at main menu, it won't immediately request a change.
            gsmain.resetStateChange();
        }
    } /******************** Handle noot transitions to and from *************************/
    else if (state==gsEnum::menutonoot) {
        gsmain.timestep(dt);
        gsnoot.timestep(dt);

        //If GSMainMenu is done with its transition animation, change state to "noot".
        if (gsmain.doneTransitioning()) {
            state=gsEnum::noot;
        }
    } else if (state==gsEnum::noot) {
        gsnoot.timestep(dt);

        if (gsnoot.getStateChange()==1) {

            state=gsEnum::noottomenu;
            gsmain.fadeIn();
            gsnoot.resetStateChange();
        }
    } else if (state==gsEnum::noottomenu) {
        gsmain.timestep(dt);
        gsnoot.timestep(dt);
        if (gsmain.doneTransitioning()) {
            state=gsEnum::mainmenu;
        }
    } 
    /******************** Handle option transitions to and from state "options" *************************/
    else if (state==gsEnum::menutooptions) {
        gsmain.timestep(dt);
        gsoptions.timestep(dt);
        if (gsmain.doneTransitioning() && gsoptions.doneTransitioning()) {
            state=gsEnum::options;
        }
    } else if (state==gsEnum::options) {
        gsoptions.timestep(dt);
        if (gsoptions.getStateChange()==1) {
            state=gsEnum::optionstomenu;
            gsmain.fadeIn();
            gsoptions.fadeOut();
            gsoptions.resetStateChange();
        }
    } else if (state==gsEnum::optionstomenu) {
        gsmain.timestep(dt);
        gsoptions.timestep(dt);
        if (gsmain.doneTransitioning() && gsoptions.doneTransitioning()) {
            state=gsEnum::mainmenu;
        }
    }
    /******************** Handle option transitions to and from state "arena" *************************/
    else if (state==gsEnum::menutoarena) {
        gsmain.timestep(dt);
        gsarena.timestep(dt);
        if (gsmain.doneTransitioning()) {
            state=gsEnum::arena;
        }
    }
    else if (state==gsEnum::arena) {
        gsarena.timestep(dt);
        if (gsarena.getStateChange()==1) {
            state=gsEnum::arenatomenu;
            gsmain.fadeIn();
            gsarena.resetStateChange();
        }
    }
    else if (state==gsEnum::arenatomenu) {
        gsmain.timestep(dt);
        gsarena.timestep(dt);
        if (gsmain.doneTransitioning()) {
            state=gsEnum::mainmenu;
        }
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
    else if (state==gsEnum::menutooptions) {
        gsoptions.render();
        gsmain.render();
    }
    else if (state==gsEnum::options) {
        gsoptions.render();
    }
    else if (state==gsEnum::optionstomenu) {
        gsoptions.render();
        gsmain.render();
    }
    else if (state==gsEnum::menutoarena) {
        gsarena.render();
        gsmain.render();
    }
    else if (state==gsEnum::arena) {
        gsarena.render();
    }
    else if (state==gsEnum::arenatomenu) {
        gsarena.render();
        gsmain.render();
    }

    audio.render();


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
    else if (state==gsEnum::menutooptions) {
        gsoptions.handleEvent(e);
        gsmain.handleEvent(e);
    }
    else if (state==gsEnum::options) {
        gsoptions.handleEvent(e);
    }
    else if (state==gsEnum::optionstomenu) {
        gsoptions.handleEvent(e);
        gsmain.handleEvent(e);
    }
    else if (state==gsEnum::menutoarena) {
        gsarena.handleEvent(e);
        gsmain.handleEvent(e);
    }
    else if (state==gsEnum::arena) {
        gsarena.handleEvent(e);
    }
    else if (state==gsEnum::arenatomenu) {
        gsarena.handleEvent(e);
        gsmain.handleEvent(e);
    }
    audio.handleEvent(e);
    if (audio.soundLevelChanged()) {
        int vol=(audio.getSoundLevel()*MIX_MAX_VOLUME)/100;
        Mix_VolumeMusic(vol);
        Mix_Volume(-1,vol);
    }
}
//window resized event callback
void GameMan::windowResized() {
    gsmain.windowResized();
    gsarena.windowResized();
    gsoptions.windowResized();
    gsnoot.windowResized();
    std::cout<<"Window resized"<<std::endl;
}