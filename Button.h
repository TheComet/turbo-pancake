#pragma once
#include <string>
#include "Texture.h"

class Button
{
	Texture pressedTexture, unpressedTexture,text,textshadow;
	int xpos, ypos;
	bool clicked; //successful "expected" click on button
	bool primed; //variable checking if cursor goes down on button initially

    int deltay; //change in height upon hover. Default is utexture.height()-ptexture.height(). Used to align the bottoms of the buttons after hovering.

    bool mouseIsOverButton();

public:
    //Null initializer: textures are empty.
    Button();

    //Initialize the button. x and y are the coordinates of the top left of the button texture.
    Button(int x,int y,Texture ptexture,Texture utexture,std::string text="");

    //Setters
    void setPos(int x,int y);
    void setdeltay(int dy);

    //Basic necessities
	void render();
	void handleEvent(SDL_Event* e);

    //Clicky logic
	bool isPressed();
	void pressReceived();
};