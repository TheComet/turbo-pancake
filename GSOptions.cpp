#include "GSOptions.h"
#include "Globals.h"


/*     Button back;
    Button placeholder1;
    Button placeholder2;

    //Variables used in a transition/fade out animation
    //state 0 is fading in, state 1 is steady displaying, state 2 is fading out, state 3 is invisible.
    int transitionstate;
    Uint32 transitionstarttime;
    const int transitionduration=1000; //1 second transitions

    int stateChange; //0 if no state change should occur, 1 if we're changing to arena, 2 if options, 3 if noot.
*/
GSOptions::GSOptions() : background(), back(),placeholder1(),placeholder2(),transitionstate(1),transitionstarttime(0),stateChange(0) { }

int GSOptions::getStateChange() {
    return stateChange;
}
void GSOptions::resetStateChange() {
    stateChange=0;
}

void GSOptions::initialize() {
    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/buttonLong_brown_pressed.png");
    Texture up=loadTexture("media/buttonLong_brown.png");
    int x=g.scWidth / 2 - p.getWidth()/2;
    int y0=100;
    int dy=p.getHeight()+30;
    placeholder1=Button(x,y0,p,up,"Placeholder");
    placeholder2=Button(x,y0+dy,p,up,"Placeholder");


    Texture p2=loadTexture("media/backbutton_pressed.png");
    Texture up2=loadTexture("media/backbutton.png");
    back=Button(g.scWidth -10 - p2.getWidth(),10,p2,up2);

    //Load the main screen image
    background=loadTexture("media/mainscreen.png");
}

//Timestep. Calls timestep on the current active game state.
void GSOptions::timestep(double dt) {
    if (transitionstate==0) {
        if (SDL_GetTicks()-transitionstarttime > transitionduration) {
            transitionstate=1;
        }
    } else if (transitionstate==1) {
        //If back button is pressed, signal that we should have a state change
        if (back.isPressed()) {
            stateChange=1;
            back.pressReceived();
        }
    }
    else if (transitionstate==2) {
        if (SDL_GetTicks() - transitionstarttime > transitionduration) {
            transitionstate = 3;
        }
    }
}
void GSOptions::render() {
    if (transitionstate==1) {
        //4 things: render background image, and the three buttons.
        background.render(g.scWidth/2-background.getWidth()/2,g.scHeight/2-background.getHeight()/2);
        back.render();
        placeholder1.render();
        placeholder2.render();
    }
    else if (transitionstate==2) {
        //Play the fading out animation
        //Same goal (render 4 things) but with a bunch of positions/transparencies varying over time.
        background.render(g.scWidth/2-background.getWidth()/2,g.scHeight/2-background.getHeight()/2);
        back.render();

        double t=double(SDL_GetTicks()-transitionstarttime)/transitionduration;


        //Some variables to ease calculation.
        double ytrans=g.scHeight;
        //Some variables pulled straight from initialize()
        int x=g.scWidth / 2 - placeholder1.getUnpressedTexture().getWidth()/2;
        int y0=100;
        int dy=placeholder1.getUnpressedTexture().getHeight()+30;


        /*
        In initialize, we call the following:
        arena=Button(x,y0,p,up,"Arena!");
        options=Button(x,y0+dy,p,up,"Options!!");
        noot=Button(x,y0+2*dy,p,up,"NOOT?!1");
        do the same thing here, but at position - t*ytrans.
        */
        t=2*t*t-t; // nice looking animation function.
        placeholder1.setPos(x,y0+t*ytrans);
        placeholder1.render();
        placeholder2.setPos(x,y0+dy+t*ytrans);
        placeholder2.render();
    }
    else if (transitionstate==0) {
        //Play the fading in animation
        //Same goal (render 4 things) but with a bunch of positions/transparencies varying over time.
        background.render(g.scWidth/2-background.getWidth()/2,g.scHeight/2-background.getHeight()/2);
        back.render();

        double t=double(SDL_GetTicks()-transitionstarttime)/transitionduration;
        t=1-t; //if we're fading in, play the same thing as for transitionstate==2 but in reverse.

        //Some variables to ease calculation.
        double ytrans=g.scHeight;
        //Some variables pulled straight from initialize()
        int x=g.scWidth / 2 - placeholder1.getUnpressedTexture().getWidth()/2;
        int y0=100;
        int dy=placeholder1.getUnpressedTexture().getHeight()+30;

        /*
        In initialize, we call the following:
        arena=Button(x,y0,p,up,"Arena!");
        options=Button(x,y0+dy,p,up,"Options!!");
        noot=Button(x,y0+2*dy,p,up,"NOOT?!1");
        do the same thing here, but at position - t*ytrans.
        */
        t=2*t*t-t; // nice looking animation function.
        placeholder1.setPos(x,y0+t*ytrans);
        placeholder1.render();
        placeholder2.setPos(x,y0+dy+t*ytrans);
        placeholder2.render();
    }
}

void GSOptions::handleEvent(SDL_Event *e) {
    if (transitionstate==1) {
        back.handleEvent(e);
        placeholder1.handleEvent(e);
        placeholder2.handleEvent(e);
    }
}
bool GSOptions::doneTransitioning() {
    return (transitionstate==1)||(transitionstate==3);
}


//Start the fading out transition: transitionstate=2.
void GSOptions::fadeOut() {
    transitionstate=2;
    transitionstarttime=SDL_GetTicks();
}

//Start the fading out transition: transitionstate=0.
void GSOptions::fadeIn() {
    transitionstate=0;
    transitionstarttime=SDL_GetTicks();
}