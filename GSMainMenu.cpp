#include "GSMainMenu.h"
#include "Globals.h"



GSMainMenu::GSMainMenu() : arena(),options(),noot(),backgroundimage(),transitionstate(1),transitionstarttime(0), stateChange(0) { }

int GSMainMenu::getStateChange() {
    return stateChange;
}

void GSMainMenu::initialize() {

    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/buttonpressed.png");
    Texture up=loadTexture("media/buttonunpressed.png");
    int x=g.scWidth / 2 - p.getWidth()/2;
    int y0=100;
    int dy=p.getHeight()+30;
    arena=Button(x,y0,p,up);
    options=Button(x,y0+dy,p,up);
    noot=Button(x,y0+2*dy,p,up);

    //Load the main screen image
    backgroundimage=loadTexture("media/mainscreen.png");
}

//Timestep. Calls timestep on the current active game state.
void GSMainMenu::timestep(double dt) { 
    if (transitionstate==0) {

    } else if (transitionstate==1) {

    } else if (transitionstate==2) {

    }
}
void GSMainMenu::render() {
    if (transitionstate==1) {
        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);
        arena.render();
        options.render();
        noot.render();
    }
}

void GSMainMenu::handleEvent(SDL_Event *e) { 
    if (transitionstate==1) {
        arena.handleEvent(e);
        options.handleEvent(e);
        noot.handleEvent(e);
    }
}