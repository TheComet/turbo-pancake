#pragma once
#include "Texture.h"
#include "Vector2.h"
class Camera {
    Vector2 pos;
    double multiplier;

public:
    Camera();

    //the linear transformation which sends the camera's position (pos) to (g.scWidth/2,g.scHeight/2),
    //while multiplying scale by multiplier.
    Vector2 worldToPixels(Vector2 w) const;

    //the inverse transformation of worldToPixels.
    Vector2 pixelsToWorld(Vector2 p) const ;

    //Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
    void renderTexture(Texture &arg,double wx,double wy,double angle,double width) const;

    //Rendering a tile ensures that the square image is drawn from the floor of the leftmost pixel 
    //to the ceil of the rightmost pixel, so that you don't get off-by-one visual errors like 
    //pixel-wide gaps between adjacent tiles.
    void renderTile(Texture &arg,double wx,double wy,double width) const;

    //getters and setters
    Vector2  getPos();
    void setPos(Vector2 arg);
    double getMultiplier();
    void setMultiplier(double arg);
};

class CameraController {
protected:
    Camera *camP;
public:
    CameraController();

    //returns cameraPointer!=nullptr
    virtual bool cameraValid();

    //sets the internal camera pointer to the argument
    virtual void attachCamera(Camera *cameraPointer);

    //sets cameraPointer to null.
    virtual void detachCamera();

    //Handle dragging mouse input. Returns true if the event was captured (any mouse click or click-drag movement).
    virtual bool handleEvent(SDL_Event *e)=0;

    //Update internal state (could be velocity, friction, etc.). Default implementation does nothing.
    virtual void timestep(double dt);
};

class FollowerCameraController : public CameraController {
    //camera velocity
    Vector2 v; 

    //exponential drag coefficient.
    double drag;

    //max velocity
    double maxv; 
    
    //how fast to pan towards the character if it is offscreen
    double panv; 
    double panacc;
    
    //true if our current goal is to center the character onscreen.
    bool goalIsToCenter; 

    /* 
    So, FollowerCameraController works like this: if the target pixel position (calculated through 
    camP->worldToPixels(targetWorldPosition)) leaves the bounding rectangle (width W, height H, 
    centered on screen), the camera feels a force proportional to targetBandStrength trying to get 
    the target back on screen. This competes with a force from the mouse which occurs if the mouse
    leaves the on screen mouseRectangle and is proportional to mouseBandStrength.
    */

    Vector2 targetWorldPosition;
    int targetRectangleW;
    int targetRectangleH;
    double targetBandStrength;

    Vector2 mousePixelPosition;
    int mouseRectangleW;
    int mouseRectangleH;
    double mouseBandStrength;

    //resets v and goalIsToCenter.
    void resetState();

    bool mouseInRectangle();
    bool targetInRectangle();
public:
    FollowerCameraController();
    
    void setTargetPosition(Vector2 pt);

    //immediately center the target.
    void forceCenterTarget();

    //transition/pan over time so that the target is centered.
    void centerTarget();

    //sets cameraPointer to null. Also zeroes internal state like camera velocity.
    void detachCamera() override;

    //Handle dragging mouse input.
    bool handleEvent(SDL_Event *e) override;

    //Update internal state (velocity, friction, etc.)
    void timestep(double dt) override;
};

class DraggingCameraController : public CameraController {

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
    DraggingCameraController();

    //sets draggable=arg.
    void setDraggable(bool arg=true); 

    //resets v, mousedragpos,mousedragpospixels,dragging, and lastdt.
    void resetState();

    //sets cameraPointer to null. Also zeroes internal state like camera velocity.
    void detachCamera() override;

    //-1 if any mouse click, 0 if left click, 1 if right click, 2 if middle click.
    void setDragButton(int arg=-1); 

    //return draggable
    bool getCameraDraggable(); 

    //Handle dragging mouse input. Returns true if the event was captured (any mouse click or click-drag movement).
    bool handleEvent(SDL_Event *e) override;

    //Update internal state (velocity, friction, etc.)
    void timestep(double dt) override;
};