#include "GSNoot.h"

GSNoot::GSNoot() : x(),y(),texts() { }

//returns stateChange
int GSNoot::getStateChange() {
    return stateChange;
}

void GSNoot::initialize() {
    texts.push_back(textTexture("noot.",{0, 0, 0}));
    texts.push_back(textTexture("Noot",{0, 0, 0}));
    texts.push_back(textTexture("NOOT",{0, 0, 0}));
    texts.push_back(textTexture("noot!",{0, 0, 0}));
    texts.push_back(textTexture("noot",{0, 0, 0}));
    for (int i=0;i<40;i++) {
        x.push_back(rand()%g.scWidth);
        y.push_back(rand()%g.scHeight);
    }
}

//Timestep. Calls timestep on the current active game state.
void GSNoot::timestep(double dt) {
    for (size_t i=0;i<x.size();i++) {
        x[i]=fmod(50*dt+x[i],g.scWidth);
        y[i]=fmod(200*dt+y[i],g.scHeight);
    }
}
void GSNoot::render() {
    for (size_t i=0;i<x.size();i++) {
        texts[i%5].render(x[i],y[i]);
    }
}
void GSNoot::handleEvent(SDL_Event *e) {
    if (e->type == SDL_KEYDOWN) {
        stateChange=1;
    }
}
