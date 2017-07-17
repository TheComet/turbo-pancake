#include "GSMainMenu.h"
#include "Globals.h"



GSMainMenu::GSMainMenu() : arena(),options(),noot(),backgroundimage(),transitionstate(1),transitionstarttime(0), stateChange(0) { }

int GSMainMenu::getStateChange() {
    return stateChange;
}

void GSMainMenu::initialize() {

    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/buttonLong_brown_pressed.png");
    Texture up=loadTexture("media/buttonLong_brown.png");
    int x=g.scWidth / 2 - p.getWidth()/2;
    int y0=100;
    int dy=p.getHeight()+30;
    arena=Button(x,y0,p,up,"Arena!");
    options=Button(x,y0+dy,p,up,"Options!!");
    noot=Button(x,y0+2*dy,p,up,"NOOT?!1");

    //Load the main screen image
    backgroundimage=loadTexture("media/mainscreen.png");
}

//Timestep. Calls timestep on the current active game state.
void GSMainMenu::timestep(double dt) { 
    if (transitionstate==0) {

    } else if (transitionstate==1) {
        if (noot.isPressed()) {
            stateChange=3;
            noot.pressReceived();
        }

    } else if (transitionstate==2) {
        if (SDL_GetTicks()-transitionstarttime > transitionduration)
            transitionstate=3;

    }
}
void GSMainMenu::render() {
    if (transitionstate==0) {
        double t=double(SDL_GetTicks()-transitionstarttime)/transitionduration;
        t=1-t; //if we're fading in, play the same thing as for transitionstate==2 but in reverse.
        
        double ytrans=g.scHeight;
        if (t>0.1)
            backgroundimage.setAlpha(floor(255*(1-(t-0.1)/0.9)));
        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);

        int x=g.scWidth / 2 - arena.getUnpressedTexture().getWidth()/2;
        int y0=100;
        int dy=arena.getUnpressedTexture().getHeight()+30;
        /* arena=Button(x,y0,p,up,"Arena!");
        options=Button(x,y0+dy,p,up,"Options!!");
        noot=Button(x,y0+2*dy,p,up,"NOOT?!1"); */
        t=2*t*t-t; // nice looking animation function.
        arena.setPos(x,y0-t*ytrans);
        arena.render();
        options.setPos(x,y0+dy-t*ytrans);
        options.render();
        noot.setPos(x,y0+2*dy-t*ytrans);
        noot.render();
    } else if (transitionstate==1) {
        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);
        arena.render();
        options.render();
        noot.render();
    } else if (transitionstate==2) {
        double t=double(SDL_GetTicks()-transitionstarttime)/transitionduration;
        double ytrans=g.scHeight;
        if(t>0.1)
            backgroundimage.setAlpha(floor(255*(1-(t-0.1)/0.9)));
        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);

        int x=g.scWidth / 2 - arena.getUnpressedTexture().getWidth()/2;
        int y0=100;
        int dy=arena.getUnpressedTexture().getHeight()+30;
        /* arena=Button(x,y0,p,up,"Arena!");
        options=Button(x,y0+dy,p,up,"Options!!");
        noot=Button(x,y0+2*dy,p,up,"NOOT?!1"); */
        t=2*t*t-t; // nice looking animation function.
        arena.setPos(x,y0-t*ytrans);
        arena.render();
        options.setPos(x,y0+dy-t*ytrans);
        options.render();
        noot.setPos(x,y0+2*dy-t*ytrans);
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
bool GSMainMenu::doneTransitioning() {
    return (transitionstate==1)||(transitionstate==3);
}


//Start the fading out transition: transitionstate=2.
void GSMainMenu::fadeOut() {
    transitionstate=2;
    transitionstarttime=SDL_GetTicks();
}

//Start the fading out transition: transitionstate=0.
void GSMainMenu::fadeIn() {
    transitionstate=0;
    transitionstarttime=SDL_GetTicks();
}