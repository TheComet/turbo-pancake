#include "Texture.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
using namespace std;

Texture::Texture() : mTexture(NULL), width(0), height(0) {}
Texture::~Texture() { free(); }
//copy ctor
Texture::Texture(const Texture& arg) : mTexture(NULL),width(0),height(0) {
    if (arg.mTexture!=NULL) {
        cout<<"Error! Copy constructor called on non-null Texture! Creating new null texture instead." <<endl;
    }
}
// Overloading of Assignment Operator
void Texture::operator=(const Texture& arg) {
    if (arg.mTexture!=NULL) {
        cout<<"Error! Assignment operator called on non-null Texture! Creating new null texture instead." <<endl;
    }
    free();
}

bool Texture::load(string path) {
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		cout<<"Unable to load image "<<path<<"! "<<IMG_GetError()<<endl;
        return false;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(g.renderer, loadedSurface);
	if (newTexture == NULL) {
        cout<<"Unable to create texture from "<<path<<"! "<<SDL_GetError()<<endl;
        return false;
	}

	//Get image dimensions
	width = loadedSurface->w;
	height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

    //save texture ptr
	mTexture = newTexture;

    //Return success
	return true;
}

bool Texture::text(std::string textureText, SDL_Color textColor) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(g.font16, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		cout<<"Unable to render text surface! "<<TTF_GetError()<<endl;
        return false;
	}

	//Create texture from surface pixels
	mTexture = SDL_CreateTextureFromSurface(g.renderer, textSurface);
	if (mTexture == NULL) {
        cout<<"Unable to create texture from rendered text! "<<SDL_GetError()<<endl;
        return false;
	}
	//Get image dimensions
	width = textSurface->w;
	height = textSurface->h;

	//Get rid of old surface
	SDL_FreeSurface(textSurface);

	//Return success
	return true;
}

void Texture::free() {
	//Free texture if it exists
	if (mTexture != NULL)
		SDL_DestroyTexture(mTexture);
	
    mTexture = NULL;
    width = 0;
    height = 0;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(g.renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}