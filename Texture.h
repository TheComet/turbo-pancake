#pragma once

#include <SDL.h>
#include <iostream>
#include <memory>
#include "SDL_ttf.h"
#include "SDL_mixer.h"




//Texture wrapper class
//Based off of http://lazyfoo.net/ tutorials w/ some modifications
class Texture {
	std::shared_ptr<SDL_Texture> mTexture;
	int width;
	int height;

	void free(); //properly deallocate mTexture
public:
	Texture();
    /*~Texture(){
        std::cout<<"ptr: "<<mTexture.get()<<std::endl;
    }*/
	bool load(std::string path);

	//Use SDL_ttf 2.0 to render text to a texture. 
	bool text(std::string textureText, SDL_Color textColor);

    //Use SDL_ttf 2.0 to render text to a texture. 
    bool text(std::string textureText,SDL_Color textColor, TTF_Font *font);

	//Set texture color
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    //explicit casts the doubles to ints and sends to render.
    void render(double x,double y,SDL_Rect* clip = NULL,double angle = 0.0,SDL_Point* center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
    //explicit casts the doubles to ints and sends to render.
    void render(int x,double y,SDL_Rect* clip = NULL,double angle = 0.0,SDL_Point* center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
    //explicit casts the doubles to ints and sends to render.
    void render(double x,int y,SDL_Rect* clip = NULL,double angle = 0.0,SDL_Point* center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderScaled(int x,int y,int w,int h,SDL_Rect* clip = NULL,double angle = 0.0,SDL_Point* center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();
    bool isNull();

    //So we can call if(texture). Texturec converts to bool and is false whenever mTexture==nullptr. 
    explicit operator bool();

};

Texture loadTexture(std::string filename);
Texture textTexture(std::string text, SDL_Color c);
Texture textTexture(std::string text,SDL_Color c,TTF_Font *font);