#include "GameMan.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>


GameMan::GameMan() : state(0),pressedbutton(),unpressedbutton(),x(),y(),texts() { srand(time(NULL)); }

//Load & initialize internal state.
void GameMan::initialize() {
    pressedbutton.load("buttonpressed.png");
    unpressedbutton.load("buttonunpressed.png");

	texts.push_back(textTexture("noot.", { 0, 0, 0 }));
	texts.push_back(textTexture("Noot", { 0, 0, 0 }));
	texts.push_back(textTexture("NOOT", { 0, 0, 0 }));
	texts.push_back(textTexture("noot!", { 0, 0, 0 }));
	texts.push_back(textTexture("noot", { 0, 0, 0 }));

	buttons.push_back(Button(g.scWidth / 2 - unpressedbutton.getWidth() / 2, g.scHeight / 2 - unpressedbutton.getHeight() / 2, loadTexture("buttonpressed.png"), loadTexture("buttonunpressed.png")));
	buttons.push_back(Button(10, 10, loadTexture("buttonpressed.png"), loadTexture("buttonunpressed.png")));

    for (int i=0;i<40;i++) {
        x.push_back(rand()%g.scWidth);
        y.push_back(rand()%g.scHeight);
    }
}

//Timestep. Calls timestep on the current active game state.
void GameMan::timestep(double dt) {

	if (state == 0) {
		for (auto it = buttons.begin(); it != buttons.end(); it++) {
			if (it->isPressed()) {
				state = 1;
				it->pressReceived();
				break;
			}
		}
	}

    if (state==1) {
        for (size_t i=0;i<x.size();i++) {
            x[i]=fmod(50*dt+x[i],g.scWidth);
            y[i]=fmod(200*dt+y[i],g.scHeight);
        }
    }
}

void GameMan::render() {
    if (state==0) {
		for (auto button : buttons) {
			button.render();
		}
    } else if (state==1) {
        for (size_t i=0;i<x.size();i++) {
            texts[i%5].render(x[i],y[i]);
        }
    }
}
void GameMan::handleEvent(SDL_Event *e) {
	//quic
	if (e->type == SDL_MOUSEBUTTONDOWN) {

	}
	if (state == 0) {
		for (auto it = buttons.begin(); it != buttons.end(); it++) {
			it -> handleEvent(e);
		}
	}
}