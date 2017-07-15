#include "GameMan.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
/*
class GameMan {
    int state; // 0 for main menu, 1 for falling text


    Texture pressedbutton;
    Texture unpressedbutton;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<Texture> texts;

    void drawButtonUnpressed();
    void drawButtonPressed();

    void freeTextures();

public:
    GameMan();
    ~GameMan();

    //Load & initialize internal state.
    void initialize();

    //Timestep. Calls timestep on the current active game state.
    void timestep(double dt);
    void render();
    void handleEvent(SDL_Event *e);*/

GameMan::GameMan() : state(0),pressedbutton(),unpressedbutton(),x(),y(),texts() { srand(time(NULL)); }

void GameMan::drawButtonUnpressed() {
    unpressedbutton.render(g.scWidth/2-unpressedbutton.getWidth()/2,g.scHeight/2-unpressedbutton.getHeight()/2);
}

void GameMan::drawButtonPressed() {
    pressedbutton.render(g.scWidth/2-pressedbutton.getWidth()/2,2+g.scHeight/2-pressedbutton.getHeight()/2);
}

//Load & initialize internal state.
void GameMan::initialize() {
    pressedbutton.load("buttonpressed.png");
    unpressedbutton.load("buttonunpressed.png");
    texts.push_back(Texture());
    texts.push_back(Texture());
    texts.push_back(Texture());
    texts.push_back(Texture());
    texts.push_back(Texture());
    texts[0].text("noot.",{0, 0, 0});
    texts[1].text("Noot",{0, 0, 0});
    texts[2].text("NOOT",{0, 0, 0});
    texts[3].text("noot!",{0, 0, 0});
    texts[4].text("noot",{0, 0, 0});
    for (int i=0;i<40;i++) {
        x.push_back(rand()%g.scWidth);
        y.push_back(rand()%g.scHeight);
    }
}

//Timestep. Calls timestep on the current active game state.
void GameMan::timestep(double dt) {
    if (state==1) {
        for (size_t i=0;i<x.size();i++) {
            x[i]=fmod(50*dt+x[i],g.scWidth);
            y[i]=fmod(200*dt+y[i],g.scWidth);
        }
    }
}
bool GameMan::mouseIsOverButton() {
    int mx,my,cx=g.scWidth/2,cy=g.scHeight/2,w=unpressedbutton.getWidth(),h=unpressedbutton.getHeight();
    SDL_GetMouseState(&mx,&my);
    return mx>cx-w/2 && mx<cx+w/2 && my<cy+h/2 && my>cy-h/2;
}
void GameMan::render() {
    if (state==0) {
        if (mouseIsOverButton()) {
            drawButtonPressed();
        } else {
            drawButtonUnpressed();
        }
    } else if (state==1) {
        for (size_t i=0;i<x.size();i++) {
            texts[i%5].render(x[i],y[i]);
        }
    }
}
void GameMan::handleEvent(SDL_Event *e) {
    if (e->type==SDL_MOUSEBUTTONUP && mouseIsOverButton()) {
        state=1;
    }
}