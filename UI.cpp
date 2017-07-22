#include "UI.h"
#include <iostream>
#include "Globals.h"


Button::Button() : pressedTexture(),unpressedTexture(),text(),textshadow(),xpos(0),ypos(0),clicked(false),primed(false),deltay(0),soundEffect() { }

//Initialize the button. x and y are the coordinates of the top left of the button texture.
//If text is left to "", no text will be displayed over the button.
//This initializer will set the click sound to g.defaultClickSound. 
Button::Button(int x,int y,Texture ptexture,Texture utexture,std::string textstr) :
    pressedTexture(ptexture),
    unpressedTexture(utexture),text(),textshadow(),
    xpos(x),
    ypos(y),
    clicked(false),
    primed(false),
    soundEffect(g.defaultClickSound) {
    deltay=unpressedTexture.getHeight()-pressedTexture.getHeight();

    if (textstr.length()>0) {
        text=textTexture(textstr,{211,191,143},g.font16bold);
        textshadow=textTexture(textstr,{98,83,73},g.font16bold);
    }
}

//Initialize the button. x and y are the coordinates of the top left of the button texture.
//This initializer will set the click sound to the argument. If the argument is the empty sound Sound(), the button will be silent. 
Button::Button(int x,int y,Texture ptexture,Texture utexture,std::string textstr,Sound clicksound) :
    pressedTexture(ptexture),
    unpressedTexture(utexture),text(),textshadow(),
    xpos(x),
    ypos(y),
    clicked(false),
    primed(false),
    soundEffect(clicksound) {
    deltay=unpressedTexture.getHeight()-pressedTexture.getHeight();

    if (textstr.length()>0) {
        text=textTexture(textstr,{211,191,143},g.font16bold);
        textshadow=textTexture(textstr,{98,83,73},g.font16bold);
    }
}

bool Button::mouseIsOverButton() {
    int mx,my,w = unpressedTexture.getWidth(),h = unpressedTexture.getHeight();
    SDL_GetMouseState(&mx,&my);
    return mx > xpos && mx < xpos + w && my < ypos + h && my > ypos;
}

void Button::render() {
    if (mouseIsOverButton()) {
        renderPressed();
    }
    else {
        renderUnpressed();
    }
}

void Button::renderPressed() {
    pressedTexture.render(xpos,ypos+deltay);
    if (!text.isNull()) {
        textshadow.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2+1,ypos+pressedTexture.getHeight()/2-text.getHeight()/2+deltay+1);
        text.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2,ypos+pressedTexture.getHeight()/2-text.getHeight()/2+deltay);
    }
}
void Button::renderUnpressed() {
    unpressedTexture.render(xpos,ypos);
    if (!text.isNull()) {
        textshadow.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2+2,ypos+pressedTexture.getHeight()/2-text.getHeight()/2+2);
        text.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2,ypos+pressedTexture.getHeight()/2-text.getHeight()/2);
    }
}

bool Button::handleEvent(SDL_Event* e) {
    if (mouseIsOverButton()) {
        if (e->type == SDL_MOUSEBUTTONUP && primed) {
            primed = false;
            clicked = true;
            playSound();
            return true;
        }
        else if (e->type == SDL_MOUSEBUTTONDOWN) {
            primed = true;
            return true;
        }
    }
    else
        primed = false;
    return false;
}

void Button::playSound() {
    //-1 selects nearest available channel, 0 repeats the sound 0 times
    soundEffect.play();
}

bool Button::isPressed() {
    return clicked;
}

void Button::pressReceived() {
    clicked = false;
}

void Button::setPos(int x,int y) {
    xpos=x; ypos=y;
}
void Button::setPos(double x,int y) { setPos(int(x),int(y)); }
void Button::setPos(int x,double y) { setPos(int(x),int(y)); }
void Button::setPos(double x,double y) { setPos(int(x),int(y)); }

void Button::setdeltay(int dy) {
    deltay=dy;
}


void Button::setAlpha(Uint8 alpha) {
    if (!unpressedTexture.isNull())
        unpressedTexture.setAlpha(alpha);
    if (!pressedTexture.isNull())
        pressedTexture.setAlpha(alpha);
    if (!text.isNull())
        text.setAlpha(alpha);
    if (!textshadow.isNull())
        textshadow.setAlpha(alpha);
}

Texture Button::getUnpressedTexture() {
    return unpressedTexture;
}
int Button::getX() {
    return xpos;
}
int Button::getY() {
    return ypos;
}
int Button::getDeltay() {
    return deltay;
}

//Null initializer: textures are empty.
ToggleButton::ToggleButton() : b(),icon(),stateChanged(false),state(false) { }

//Initialize the button. x and y are the coordinates of the top left of the button texture.
//If text is left to "", no text will be displayed over the button.
//This initializer will set the click sound to g.defaultClickSound. 
ToggleButton::ToggleButton(int x,int y,Texture ptexture,Texture utexture,std::string text,Texture uicon)
: b(x,y,ptexture,utexture,text), icon(uicon),stateChanged(false),state(false){ }

//Initialize the button. x and y are the coordinates of the top left of the button texture.
//This initializer will set the click sound to the argument. If the argument is the empty sound Sound(), the button will be silent. 
ToggleButton::ToggleButton(int x,int y,Texture ptexture,Texture utexture,std::string text,Texture uicon,Sound uclicksound) : b(x,y,ptexture,utexture,text,uclicksound),icon(uicon),stateChanged(false),state(false) { }

void ToggleButton::setState(bool arg) {
    state=arg;
    stateChanged=true;
}

//Setters
void ToggleButton::setPos(int x,int y) { b.setPos(x,y); }
void ToggleButton::setPos(double x,int y) { b.setPos(x,y); }
void ToggleButton::setPos(int x,double y) { b.setPos(x,y); }
void ToggleButton::setPos(double x,double y) { b.setPos(x,y); }
void ToggleButton::setdeltay(int dy) { b.setdeltay(dy); }

void ToggleButton::setAlpha(Uint8 alpha) { b.setAlpha(alpha); icon.setAlpha(alpha); }

//Basic necessities
void ToggleButton::render() { 
    int dy=0;
    if (state) {
        b.renderPressed();
        dy=b.getDeltay();
    }
    else {
        b.renderUnpressed();
    }
    if (icon) {
        icon.render(b.getX()+b.getUnpressedTexture().getWidth()/2-icon.getWidth()/2,
            b.getY()+b.getUnpressedTexture().getHeight()/2-icon.getHeight()/2+dy);
    }

    if (b.isPressed()) {
        stateChanged=true;
        state=!state;
        b.pressReceived();
    }
}

bool ToggleButton::wasUpdated() { return stateChanged; }
void ToggleButton::updateReceived() { stateChanged=false; }
bool ToggleButton::getState() { return state; }

//handleEvent returns true if mouse input was "captured" by the GUI element.
//This can be used by Game States or other utilities to eg realize a mouse
//clicked a button and not the map!
bool ToggleButton::handleEvent(SDL_Event* e) { return b.handleEvent(e); }
void ToggleButton::playSound() { b.playSound(); }
Texture ToggleButton::getUnpressedTexture() { return b.getUnpressedTexture(); }
int ToggleButton::getX() { return b.getY(); }
int ToggleButton::getY() { return b.getX();  }
int ToggleButton::getDeltay() { return b.getDeltay(); }











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
    return 0;
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
bool AudioWidget::handleEvent(SDL_Event* e) {
    if (!primed) {
        if (e->type == SDL_MOUSEBUTTONDOWN && mouseIsOverButton()) {
            primed = true;
            dragged=false;
            xprimed=e->button.x;
            yprimed=e->button.y;
            soundlevelprimed=soundlevel;
            return true;
        } 
    }
    else {//primed is true.
        if (!dragged && e->type==SDL_MOUSEMOTION) {
            int mx=e->motion.x;
            int my=e->motion.y;
            if (abs(mx-xprimed)+abs(my-yprimed) >2) {
                dragged=true;
            }
            return true;
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
            return true;
        }
        else if (!dragged && e->type==SDL_MOUSEBUTTONUP) {
            primed=false;
            dragged=false;
            xprimed=0; yprimed=0; soundlevelprimed=0;
            soundchanged=true;
            toggle=!toggle;
            if (toggle && soundlevel==0)
                soundlevel=50;
        }
        else if (dragged && e->type==SDL_MOUSEBUTTONUP) {
            primed=false;
            dragged=false;
            xprimed=0; yprimed=0; soundlevelprimed=0;
            soundchanged=true;
        }
    }
    return false;
}


void AudioWidget::playSound() {
    if(soundEffect)
        soundEffect.play();
}

Texture AudioWidget::getUnpressedTexture() {
    if (graphics.size()!=4)
        return Texture();
    
    return graphics[0];
}

int AudioWidget::getX() { return ypos; }
int AudioWidget::getY() { return xpos;  }


