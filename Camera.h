#pragma once
#include "Texture.h"
#include "Vector2.h"
class Camera {
    Vector2 pos;
    double multiplier;

    bool cameradraggable; //if false, handleEvent does nothing.

    Vector2 v; //camera velocity for click and drag releases
    double drag,maxv; // max velocity

    Vector2 mousedragpos; //x and y when the mouse was clicked
    Vector2 mousedragpospixels; //x and y of the mouse when the mouse was clicked
    Vector2 lastpos;//x and y the last frame. Used for calculating final velocity. 

    bool dragging; //true if the mouse has been clicked and held.
    int dragbutton; //-1 if any mouse click, 0 if left click, 1 if right click, 2 if middle click.
    double lastdt;
public:
    Camera();

    Vector2 worldToPixels(Vector2 w) const;
    Vector2 pixelsToWorld(Vector2 p) const ;


    //Ensures that the point at (x,y) is at the center of the screen.
    //Note that the center of the tile at (0,0) has coordinates (0.5,0.5).
    void centerCameraOnTile(double x,double y);

    void setDraggable(bool arg=true); //draggable=true
    void setDragButton(int arg=-1); //-1 if any mouse click, 0 if left click, 1 if right click, 2 if middle click.
    bool getCameraDraggable(); //return draggable

    //Core things: events & movement!
    bool handleEvent(SDL_Event *e);
    void timestep(double dt);

    //Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
    void renderTexture(Texture &arg,double wx,double wy,double angle,double width) const;

    //Rendering a tile ensures that the square image is drawn from the floor of the leftmost pixel 
    //to the ceil of the rightmost pixel, so that you don't get off-by-one visual errors like 
    //pixel-wide gaps between adjacent tiles.
    void renderTile(Texture &arg,double wx,double wy,double width) const;
};