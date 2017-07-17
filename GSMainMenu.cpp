#include "GSMainMenu.h"
#include "Globals.h"



GSMainMenu::GSMainMenu() : arena(),options(),noot(),backgroundimage(),transitionstate(1),transitionstarttime(0), stateChange(0) { }

int GSMainMenu::getStateChange() {
    return stateChange;
}

void GSMainMenu::initialize() {

    menumusic.load("media/audio/Wacky Waiting.ogg");

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
        //Fade in music if it's not already playing
        if (!menumusic.playing()) {
            menumusic.fadeIn();
        }
    } else if (transitionstate==1) {
        //Fade in music if it's not already playing
        if (!menumusic.playing()) {
            menumusic.fadeIn();
        }

        //If noot button is pressed, fade out music and signal that we should have a state change
        if (noot.isPressed()) {
            stateChange=3;
            menumusic.fadeOut();
            noot.pressReceived();
        }
    } else if (transitionstate==2) {

		if (SDL_GetTicks() - transitionstarttime > transitionduration) {
			transitionstate = 3;
		}
    }
}
void GSMainMenu::render() {
    if (transitionstate==1) {
        //4 things: render background image, and the three buttons.

        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);
        arena.render();
        options.render();
        noot.render();

    } else if (transitionstate==2) { 
        //Play the fading out animation
        //Same goal (render 4 things) but with a bunch of positions/transparencies varying over time.

        double t=double(SDL_GetTicks()-transitionstarttime)/transitionduration;

        //Some variables to ease calculation.
        double ytrans=g.scHeight;
        //Some variables pulled straight from initialize()
        int x=g.scWidth / 2 - arena.getUnpressedTexture().getWidth()/2;
        int y0=100;
        int dy=arena.getUnpressedTexture().getHeight()+30;

        //Draw the background button
        if(t>0.1)
            backgroundimage.setAlpha(floor(255*(1-(t-0.1)/0.9)));
        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);

        /* 
        In initialize, we call the following:
        arena=Button(x,y0,p,up,"Arena!");
        options=Button(x,y0+dy,p,up,"Options!!");
        noot=Button(x,y0+2*dy,p,up,"NOOT?!1"); 
        do the same thing here, but at position - t*ytrans.
        */
        t=2*t*t-t; // nice looking animation function.
        arena.setPos(x,y0-t*ytrans);
        arena.render();
        options.setPos(x,y0+dy-t*ytrans);
        options.render();
        noot.setPos(x,y0+2*dy-t*ytrans);
        noot.render();
    } else if (transitionstate==0) {
        //Play the fading in animation
        //Same goal (render 4 things) but with a bunch of positions/transparencies varying over time.

        double t=double(SDL_GetTicks()-transitionstarttime)/transitionduration;
        t=1-t; //if we're fading in, play the same thing as for transitionstate==2 but in reverse.

               //Some variables to ease calculation.
        double ytrans=g.scHeight;
        //Some variables pulled straight from initialize()
        int x=g.scWidth / 2 - arena.getUnpressedTexture().getWidth()/2;
        int y0=100;
        int dy=arena.getUnpressedTexture().getHeight()+30;

        //Draw the background button
        if (t>0.1)
            backgroundimage.setAlpha(floor(255*(1-(t-0.1)/0.9)));
        backgroundimage.render(g.scWidth/2-backgroundimage.getWidth()/2,g.scHeight/2-backgroundimage.getHeight()/2);

        /*
        In initialize, we call the following:
        arena=Button(x,y0,p,up,"Arena!");
        options=Button(x,y0+dy,p,up,"Options!!");
        noot=Button(x,y0+2*dy,p,up,"NOOT?!1");
        do the same thing here, but at position - t*ytrans.
        */
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