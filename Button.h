#pragma once
#include <string>
#include "Texture.h"

class Button
{
	Texture pressedTexture, unpressedTexture;
	int xpos, ypos;
	bool clicked; //successful "expected" click on button
	bool primed; //variable checking if cursor goes down on button initially

public:
	Button(int x, int y, Texture ptexture, Texture utexture) : pressedTexture(Texture()), unpressedTexture(Texture()), clicked(false), primed(false)  {
		xpos = x;
		ypos = y;
		pressedTexture = ptexture;
		unpressedTexture = utexture;
	}

	~Button() {}

	bool mouseIsOverButton();
	void render();
	void handleEvent(SDL_Event* e);
	bool isPressed();
	void pressReceived();
};