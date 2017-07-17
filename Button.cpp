#include "Button.h"
#include <iostream>
#include "Globals.h"

Button::Button() : pressedTexture(),unpressedTexture(),text(),textshadow(),xpos(0),ypos(0),clicked(false),primed(false),deltay(0),soundEffect() { }

//Initialize the button. x and y are the coordinates of the top left of the button texture.
//If text is left to "", no text will be displayed over the button.
//This initializer will set the click sound to g.defaultClickSound. 
Button::Button(int x,int y,Texture ptexture,Texture utexture, std::string textstr) : 
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
	int mx, my, w = unpressedTexture.getWidth(), h = unpressedTexture.getHeight();
	SDL_GetMouseState(&mx, &my);
	return mx > xpos && mx < xpos + w && my < ypos + h && my > ypos;
}

void Button::render() {
    if (mouseIsOverButton()) {
        pressedTexture.render(xpos,ypos+deltay);
        if (!text.isNull()) {
            textshadow.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2+1,ypos+pressedTexture.getHeight()/2-text.getHeight()/2+deltay+1);
            text.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2,ypos+pressedTexture.getHeight()/2-text.getHeight()/2+deltay);
        }
    } else {
        unpressedTexture.render(xpos,ypos);
        if (!text.isNull()) {
            textshadow.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2+2,ypos+pressedTexture.getHeight()/2-text.getHeight()/2+2);
            text.render(xpos+pressedTexture.getWidth()/2-text.getWidth()/2,ypos+pressedTexture.getHeight()/2-text.getHeight()/2);
        }
    }
}

void Button::handleEvent(SDL_Event* e) {
	if (mouseIsOverButton()) {
		if (e->type == SDL_MOUSEBUTTONUP && primed) {
			primed = false;
			clicked = true;
			playSound();
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
			primed = true;
	}
	else
		primed = false;
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

void Button::setPos(int x, int y) {
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

Texture & Button::getUnpressedTexture() {
    return unpressedTexture;
}
int Button::getX() {
    return xpos;
}
int Button::getY() {
    return ypos;
}