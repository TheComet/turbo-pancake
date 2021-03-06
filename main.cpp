#include "Globals.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "GameMan.h"
#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <SDL_mixer.h>

using namespace std;
Globals g;

/* Start up SDL and create the window and renderer. Also initialize SDL_image for loading pngs,
 * and SDL_TTF for loading fonts. Also load the global TTF font! */
bool init() {
    //Window size to request.
    g.scWidth=1280;
    g.scHeight=800;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout<<"SDL could not initialize! "<<SDL_GetError()<<endl;
		return false;
	}

	//Initialize audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Set texture filtering
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2")) {
        cout<<"Warning: texture filtering not set correctly!"<<endl;
	}

	//Create window
	g.window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g.scWidth, g.scHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (g.window == NULL) {
        cout<<"Window could not be created! "<<SDL_GetError()<<endl;
        return false;
	}
    g.mousefocus = true;
    g.keyboardfocus = true;

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
    g.font16 = TTF_OpenFont("media/Vera.ttf",16);
    if (g.font16==NULL) {
        cout<<TTF_GetError()<<endl;
        return false;
    }
    g.font28 = TTF_OpenFont("media/Vera.ttf",28);
    if (g.font28==NULL) {
        cout<<TTF_GetError()<<endl;
        return false;
    }
    g.font16bold = TTF_OpenFont("media/VeraBd.ttf",16);
    if (g.font16bold==NULL) {
        cout<<TTF_GetError()<<endl;
        return false;
    }


    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
        cout<<"SDL_mixer could not initialize! "<<Mix_GetError()<<endl;
        return false;
    }
    //load main button sounds
    g.defaultClickSound=Sound();
    g.defaultClickSound.load("media/audio/knifeSlice.ogg");

    g.bloodsplat=loadTexture("media/bloodsplat.png");
    g.bloodsplat.setAlpha(200);

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
	Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc,char* args[]) {

    srand((unsigned int)time(NULL));
    if (!init()) {
        cout<<"Initialization failed."<<endl;
        cin.get();
        return 1;
    }


    {
        //Adding a block here forces GameMan to deallocate BEFORE close is called. 
        //So all the textures/sounds get destructed, and THEN we call Mix_Quit. 
        GameMan game;
        game.initialize();

        Uint32 lasttime = SDL_GetTicks();

        Mix_VolumeMusic(0);
        Mix_Volume(-1,0);

        //main loop
        while (!g.quit) {
            g.mouseCapturedByGUI=false;

            //Handle events on queue
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0) {
                //User requests quit
                if (e.type == SDL_QUIT) {
                    g.quit = true;
                }
                else if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym==SDLK_ESCAPE)
                        g.quit=true;
                    else if (e.key.keysym.sym==SDLK_w)
                        g.wdown=true;
                    else if (e.key.keysym.sym==SDLK_a)
                        g.adown=true;
                    else if (e.key.keysym.sym==SDLK_s)
                        g.sdown=true;
                    else if (e.key.keysym.sym==SDLK_d)
                        g.ddown=true;
                } 
                else if (e.type==SDL_KEYUP) {
                    if (e.key.keysym.sym==SDLK_w)
                        g.wdown=false;
                    else if (e.key.keysym.sym==SDLK_a)
                        g.adown=false;
                    else if (e.key.keysym.sym==SDLK_s)
                        g.sdown=false;
                    else if (e.key.keysym.sym==SDLK_d)
                        g.ddown=false;
                }
                else if (e.type== SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_SIZE_CHANGED) {
                    g.scWidth = e.window.data1;
                    g.scHeight = e.window.data2;
                    game.windowResized();
                }
               
                game.handleEvent(&e);
            }
            const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
            g.wdown=(keyboardState[SDL_SCANCODE_W]==1);
            g.adown=(keyboardState[SDL_SCANCODE_A]==1);
            g.sdown=(keyboardState[SDL_SCANCODE_S]==1);
            g.ddown=(keyboardState[SDL_SCANCODE_D]==1);

            
            SDL_GetMouseState(&g.mousex,&g.mousey);

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
    }
    close();

    return 0;
}