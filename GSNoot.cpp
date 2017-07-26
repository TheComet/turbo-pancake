#include "GSNoot.h"
#include "Globals.h"

GSNoot::GSNoot() : back(),x(),y(),texts(),nootmusic(),stateChange(0) { }

//returns stateChange
int GSNoot::getStateChange() {
    return stateChange;
}
void GSNoot::resetStateChange() {
    stateChange=0;
}

void GSNoot::initialize() {
    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/backbutton_pressed.png");
    Texture up=loadTexture("media/backbutton.png");

    nootmusic.load("media/audio/Mission Plausible.ogg");

    Sound buttonsound;
    buttonsound.load("media/audio/iCannotDoThat.ogg");
    back=Button(g.scWidth -10 - p.getWidth(),10,p,up,"",buttonsound);

    texts.push_back(textTexture("noot.",{0, 0, 0}));
    texts.push_back(textTexture("Noot",{0, 100, 0}));
    texts.push_back(textTexture("NOOT",{100, 0, 0}));
    texts.push_back(textTexture("noot!",{0, 0, 100}));
    texts.push_back(textTexture("noot",{100,100, 0}));
    for (int i=0;i<80;i++) {
        x.push_back(rand()% (g.scWidth+200));
        y.push_back(rand()% (g.scHeight+200));
    }
}

//Timestep. Calls timestep on the current active game state.
void GSNoot::timestep(double dt) {
    if (!nootmusic.playing()) {
        nootmusic.fadeIn();
    }
    for (size_t i=0;i<x.size();i++) {
        x[i]=fmod(50*dt+x[i],g.scWidth+200);
        y[i]=fmod(200*dt+y[i],g.scHeight+200);
    }
    if (back.isPressed()) {
        stateChange=1;
        nootmusic.fadeOut();
        back.pressReceived();
    }
}
void GSNoot::render() {
    for (size_t i=0;i<x.size();i++) {
        texts[i%5].render(x[i]-100,y[i]-100);
    }
    back.render();
}
void GSNoot::handleEvent(SDL_Event *e) {

    back.handleEvent(e);
}


//window resized event callback
void GSNoot::windowResized() {
    back.setPos(g.scWidth -10 - back.getUnpressedTexture().getWidth(),10);
    for (size_t i=0;i<x.size();i++) {
        x[i]=(rand()% (g.scWidth+200));
        y[i]=(rand()% (g.scHeight+200));
    }
}