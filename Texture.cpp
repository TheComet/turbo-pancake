#include "Texture.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
using namespace std;

Texture::Texture() : mTexture(nullptr), width(0), height(0) {}
Texture::~Texture() { }



bool Texture::load(string path) {
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		cout<<"Unable to load image "<<path<<"! "<<IMG_GetError()<<endl;
        return false;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(g.renderer, loadedSurface);
	if (newTexture == nullptr) {
        cout<<"Unable to create texture from "<<path<<"! "<<SDL_GetError()<<endl;
        return false;
	}

	//Get image dimensions
	width = loadedSurface->w;
	height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

    //save texture ptr
	mTexture = std::shared_ptr<SDL_Texture>(newTexture, SDL_DestroyTexture);

    //Return success
	return true;
}

bool Texture::text(std::string textureText, SDL_Color textColor) {
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(g.font16, textureText.c_str(), textColor);
	if (textSurface == nullptr) {
		cout<<"Unable to render text surface! "<<TTF_GetError()<<endl;
        return false;
	}

	//Create texture from surface pixels
	mTexture =std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(g.renderer, textSurface),SDL_DestroyTexture);
	if (mTexture == nullptr) {
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
    mTexture = nullptr;
    width = 0;
    height = 0;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture.get(), red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture.get(), blending);
}

void Texture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture.get(), alpha);
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
	SDL_RenderCopyEx(g.renderer, mTexture.get(), clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

Texture loadTexture(std::string filename){
	Texture t;
	t.load(filename);
	return t;
}
Texture textTexture(std::string text, SDL_Color c){
	Texture t;
	t.text(text, c);
	return t;
}