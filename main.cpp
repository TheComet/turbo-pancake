#include "Globals.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "GameMan.h"
#include <iostream>
#include <string>

using namespace std;
Globals g;

/* Start up SDL and create the window and renderer. Also initialize SDL_image for loading pngs,
 * and SDL_TTF for loading fonts. Also load the global TTF font! */
bool init() {
    //Window size to request.
    g.scWidth=640;
    g.scHeight=480;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout<<"SDL could not initialize! "<<SDL_GetError()<<endl;
		return false;
	}

	//Set texture filtering
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
        cout<<"Warning: texture filtering not set correctly!"<<endl;
	}

	//Create window
	g.window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g.scWidth, g.scHeight, SDL_WINDOW_SHOWN);
	if (g.window == NULL) {
        cout<<"Window could not be created! "<<SDL_GetError()<<endl;
        return false;
	}

	//Create a vsynced SDL renderer for window.
    //this takes care of graphics acceleration for us!
	g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g.renderer == NULL) {
        cout<<"Renderer could not be created! "<<SDL_GetError()<<endl;
        return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(g.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		cout<<"SDL_image could not initialize! "<<IMG_GetError()<<endl;
        return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		cout<<"SDL_ttf could not initialize! "<<TTF_GetError()<<endl;
        return false;
	}
    

    //Open the font
    g.font16 = TTF_OpenFont("Vera.ttf",16);
    if (g.font16==NULL) {
        cout<<TTF_GetError()<<endl;
        return false;
    }
    g.font28 = TTF_OpenFont("Vera.ttf",28);
    if (g.font28==NULL) {
        cout<<TTF_GetError()<<endl;
        return false;
    }

	return true;
}

void close() {
    //Free global font
    TTF_CloseFont(g.font16);
    TTF_CloseFont(g.font28);
    g.font16=NULL;
    g.font28=NULL;

    //Destroy window	
    SDL_DestroyRenderer(g.renderer);
    SDL_DestroyWindow(g.window);
    g.window = NULL;
    g.renderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/*
Texture text;
//Render text
SDL_Color textColor ={0, 0, 0};
text.text("The quick brown fox jumps over the lazy dog",textColor);

        //Render current frame
        text.render((g.scWidth - text.getWidth()) / 2,(g.scHeight - text.getHeight()) / 2);
*/
int main(int argc,char* args[]) {


    if (!init()) {
        cout<<"Initialization failed."<<endl;
        cin.get();
        return 1;
    }

    GameMan game;
    game.initialize();

    Uint32 lasttime = SDL_GetTicks();
    

    //main loop
    while (!g.quit) {
        //Handle events on queue
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                g.quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym==SDLK_ESCAPE)
                    g.quit=true;
            }
            game.handleEvent(&e);
        }

        Uint32 newtime= SDL_GetTicks();
        game.timestep((newtime-lasttime)*0.001);
        lasttime=newtime;

        //Clear screen
        SDL_SetRenderDrawColor(g.renderer,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderClear(g.renderer);

        game.render();

        //Update screen
        SDL_RenderPresent(g.renderer);
    }


    close();

    return 0;
}