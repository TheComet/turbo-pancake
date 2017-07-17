#include "Button.h"
#include <iostream>


Button::Button() : pressedTexture(),unpressedTexture(),xpos(0),ypos(0),clicked(false),primed(false) { }

Button::Button(int x,int y,Texture ptexture,Texture utexture): pressedTexture(Texture()),unpressedTexture(Texture()),clicked(false),primed(false)  {
    xpos = x;
    ypos = y;
    pressedTexture = ptexture;
    unpressedTexture = utexture;
}
bool Button::mouseIsOverButton() {
	int mx, my, w = unpressedTexture.getWidth(), h = unpressedTexture.getHeight();
	SDL_GetMouseState(&mx, &my);
	return mx > xpos && mx < xpos + w && my < ypos + h && my > ypos;
}

void Button::render() {
	if (mouseIsOverButton())
		pressedTexture.render(xpos, ypos);
	else
		unpressedTexture.render(xpos, ypos);
}

void Button::handleEvent(SDL_Event* e) {
	if (mouseIsOverButton()) {
		if (e->type == SDL_MOUSEBUTTONUP && primed) {
			primed = false;
			clicked = true;
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN)
			primed = true;
	}
	else
		primed = false;
}

bool Button::isPressed() {
	return clicked;
}

void Button::pressReceived() {
	clicked = false;
}