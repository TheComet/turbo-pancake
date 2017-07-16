#pragma once

#include <SDL.h>
#include "Globals.h"
#include <iostream>
#include <memory>




//Texture wrapper class
//Based off of http://lazyfoo.net/ tutorials w/ some modifications
class Texture {
	std::shared_ptr<SDL_Texture> mTexture;
	int width;
	int height;

	void free(); //properly deallocate mTexture
public:
	Texture();
	~Texture();	

	bool load(std::string path);
    //color keying options possible: ,bool colorKey=false,Uint8 r=0,Uint8 g=0xFF,Uint8 b=0xFF

	//Use SDL_ttf 2.0 to render text to a texture. 
	bool text(std::string textureText, SDL_Color textColor);

	//Set texture color
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();
};

Texture loadTexture(std::string filename);
Texture textTexture(std::string text, SDL_Color c);