#pragma once
#include <string>
#include "Texture.h"

class Button
{
	Texture pressedTexture, unpressedTexture;
	int xpos, ypos;
	bool clicked;

public:
	Button(int x, int y, Texture ptexture, Texture utexture) : pressedTexture(Texture()), unpressedTexture(Texture()), clicked(false)  {
		xpos = x;
		ypos = y;
		pressedTexture = ptexture;
		unpressedTexture = utexture;
	}

	~Button() {}

	bool mouseIsOverButton();
	/*void drawButtonPressed();
	void drawButtonUnpressed(); */
	void render();
	void handleEvent(SDL_Event* e);
	bool isPressed();
};