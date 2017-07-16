#include "Button.h"


bool Button::mouseIsOverButton() {
	int mx, my, cx = g.scWidth / 2, cy = g.scHeight / 2, w = unpressedTexture.getWidth(), h = unpressedTexture.getHeight();
	SDL_GetMouseState(&mx, &my);
	return mx>cx - w / 2 && mx<cx + w / 2 && my<cy + h / 2 && my>cy - h / 2;
}/*
void Button::drawButtonUnpressed() {
	unpressedTexture.render(xpos, ypos);
}

void Button::drawButtonPressed() {
	pressedTexture.render(xpos, ypos);
} */

void Button::render() {
	if (mouseIsOverButton())
		pressedTexture.render(xpos, ypos);
	else
		unpressedTexture.render(xpos, ypos);
}

void Button::handleEvent(SDL_Event* e) {
	if (e->type == SDL_MOUSEBUTTONUP && mouseIsOverButton())
		clicked = true;
}

bool Button::isPressed() {
	bool temp = clicked;
	clicked = false;
	return temp;
}