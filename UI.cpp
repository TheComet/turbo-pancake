#include "UI.h"

#include "Button.h"
#include <iostream>
#include "Globals.h"

//Null initializer: textures are empty.
AudioWidget::AudioWidget() : graphics(),xpos(0),ypos(0),dragged(false),primed(false),xprimed(0),yprimed(0),soundlevelprimed(0),toggle(false),soundlevel(0),soundchanged(true),deltay(0),soundEffect() { };

//Initialize the button. x and y are the coordinates of the top left of the button texture.
//If text is left to "", no text will be displayed over the button.
//This initializer will set the click sound to g.defaultClickSound. 
AudioWidget::AudioWidget(int x,int y) : graphics(),xpos(x),ypos(y),dragged(false),primed(false),xprimed(0),yprimed(0),soundlevelprimed(0),toggle(false),soundlevel(0),soundchanged(true),deltay(0),soundEffect() {
    graphics.push_back(loadTexture("media/audio0.png"));
    graphics.push_back(loadTexture("media/audio1.png"));
    graphics.push_back(loadTexture("media/audio2.png"));
    graphics.push_back(loadTexture("media/audio3.png"));
    soundEffect=g.defaultClickSound;
};

bool AudioWidget::mouseIsOverButton() {
    if (graphics.size()!=4)
        return false;
    int mx,my,w = graphics[0].getWidth(),h = graphics[0].getHeight();
    SDL_GetMouseState(&mx,&my);
    return mx > xpos && mx < xpos + w && my < ypos + h && my > ypos;
}



//reading data. getSoundLevel sets soundchanged to false.
int AudioWidget::getSoundLevel() {
    soundchanged=false;
    if (toggle)
        return soundlevel;
    return false;
}
//Check if soundlevel has been changed since getSoundLevel was last read.
bool AudioWidget::soundLevelChanged() {
    return soundchanged;
}


void AudioWidget::setPos(int x,int y) {
    xpos=x; ypos=y;
}
void AudioWidget::setPos(double x,int y) { setPos(int(x),int(y)); }
void AudioWidget::setPos(int x,double y) { setPos(int(x),int(y)); }
void AudioWidget::setPos(double x,double y) { setPos(int(x),int(y)); }
void AudioWidget::setdeltay(int dy) {
    deltay=dy;
}


//Basic necessities
void AudioWidget::render() {
    if (graphics.size()!=4)
        return;
    if (toggle) {
        if (soundlevel<25)
            graphics[0].render(xpos,ypos);
        else if (soundlevel<50)
            graphics[1].render(xpos,ypos);
        else if (soundlevel<75)
            graphics[2].render(xpos,ypos);
        else
            graphics[3].render(xpos,ypos);
    }
    else {
        graphics[0].render(xpos,ypos);
    }
}
void AudioWidget::handleEvent(SDL_Event* e) {
    std::cout<<dragged<<" "<<primed<<std::endl;
    if (!primed) {
        if (e->type == SDL_MOUSEBUTTONDOWN && mouseIsOverButton()) {
            primed = true;
            dragged=false;
            xprimed=e->button.x;
            yprimed=e->button.y;
            soundlevelprimed=soundlevel;
        } 
    }
    else {//primed is true.
        if (!dragged && e->type==SDL_MOUSEMOTION) {
            int mx=e->motion.x;
            int my=e->motion.y;
            if (abs(mx-xprimed)+abs(my-yprimed) >2) {
                dragged=true;
            }
        }
        else if (dragged && e->type==SDL_MOUSEMOTION) {
            int mx=e->motion.x;
            int my=e->motion.y;
            int deltalevel=mx-xprimed;
            soundlevel=soundlevelprimed+deltalevel;
            toggle=true; 
            if (soundlevel<0)
                soundlevel=0;
            if (soundlevel>100)
                soundlevel=100;
            soundchanged=true;
        }
        else if (!dragged && e->type==SDL_MOUSEBUTTONUP) {
            primed=false;
            dragged=false;
            xprimed=0; yprimed=0; soundlevelprimed=0;
            soundchanged=true;
            toggle=!toggle;
        }
        else if (dragged && e->type==SDL_MOUSEBUTTONUP) {
            primed=false;
            dragged=false;
            xprimed=0; yprimed=0; soundlevelprimed=0;
            soundchanged=true;
        }
    }
}


void AudioWidget::playSound() {
    if(soundEffect)
        soundEffect.play();
}

Texture & AudioWidget::getUnpressedTexture() {
    if (graphics.size()!=4)
        return Texture();
    
    return graphics[0];
}

int AudioWidget::getX() { return ypos; }
int AudioWidget::getY() { return xpos;  }


