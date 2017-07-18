#pragma once
#include "Texture.h"
#include "Sound.h"
#include <iostream>
#include <string>
#include <vector>

class Button
{
    Texture pressedTexture,unpressedTexture,text,textshadow;
    int xpos,ypos;
    bool clicked; //successful "expected" click on button
    bool primed; //variable checking if cursor goes down on button initially

    int deltay; //change in height upon hover. Default is utexture.height()-ptexture.height(). Used to align the bottoms of the buttons after hovering.

    Sound soundEffect; //sound to be played on click

    bool mouseIsOverButton();

public:
    //Null initializer: textures are empty.
    Button();

    //Initialize the button. x and y are the coordinates of the top left of the button texture.
    //If text is left to "", no text will be displayed over the button.
    //This initializer will set the click sound to g.defaultClickSound. 
    Button(int x,int y,Texture ptexture,Texture utexture,std::string text = "");

    //Initialize the button. x and y are the coordinates of the top left of the button texture.
    //This initializer will set the click sound to the argument. If the argument is the empty sound Sound(), the button will be silent. 
    Button(int x,int y,Texture ptexture,Texture utexture,std::string text,Sound clicksound);

    //Setters
    void setPos(int x,int y);
    void setPos(double x,int y);
    void setPos(int x,double y);
    void setPos(double x,double y);
    void setdeltay(int dy);

    void setAlpha(Uint8 alpha);

    //Basic necessities
    void render();
    void handleEvent(SDL_Event* e);

    //Clicky logic
    bool isPressed();
    void pressReceived();

    void playSound();

    Texture & getUnpressedTexture();

    int getX();
    int getY();
};


class AudioWidget {
    //Audio speaker graphic: 0 bars, 1 bar, 2 bars, 3 bars.
    std::vector<Texture> graphics; 

    //top left corner of the sprite
    int xpos,ypos;

    /* Only two state variables for mouse countrol: primed and dragged.
    Primed is set to true whenever the user hovers over the button and clicks.
    Dragged is set to true if and only if primed is true and the mouse has moved more than two pixels. */
    bool dragged;
    bool primed;
    int xprimed,yprimed,soundlevelprimed;

    //false if muted, true if unmuted.
    bool toggle; 
    //Sound level! 0 is no volume, 100 is full volume.
    int soundlevel; 
    //true if soundlevel has been changed since it was last read.
    bool soundchanged;

    int deltay; //change in height upon hover. Default is utexture.height()-ptexture.height(). Used to align the bottoms of the buttons after hovering.

    Sound soundEffect; //sound to be played on click

    bool mouseIsOverButton();

public:
    //Null initializer: textures are empty.
    AudioWidget();

    //Initialize the button. x and y are the coordinates of the top left of the button texture.
    //If text is left to "", no text will be displayed over the button.
    //This initializer will set the click sound to g.defaultClickSound. 
    AudioWidget(int x,int y);
    

    //Setters
    void setPos(int x,int y);
    void setPos(double x,int y);
    void setPos(int x,double y);
    void setPos(double x,double y);
    void setdeltay(int dy);
    void playSound();

    //Basic necessities
    void render();
    void handleEvent(SDL_Event* e);

    //reading data. getSoundLevel sets soundchanged to false.
    int getSoundLevel();
    //Check if soundlevel has been changed since getSoundLevel was last read.
    bool soundLevelChanged();

    Texture & getUnpressedTexture();

    int getX();
    int getY();
};
