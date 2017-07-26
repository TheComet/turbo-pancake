#include "Camera.h"
#include <cmath>  
#include "Globals.h"


Camera::Camera() : pos(),multiplier(50) { }

Vector2 Camera::worldToPixels(Vector2 w) const {
    return Vector2((w.x-pos.x)*multiplier+g.scWidth/2,(w.y-pos.y)*multiplier+g.scHeight/2);
}

Vector2 Camera::pixelsToWorld(Vector2 p) const {
    return Vector2((p.x-g.scWidth/2)/multiplier+pos.x,(p.y-g.scHeight/2)/multiplier+pos.y);
}

//Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
void Camera::renderTexture(Texture &arg,double wx,double wy,double angle,double width) const {
    arg.renderScaled((int)((wx-pos.x)*multiplier+g.scWidth/2-width/2.0*multiplier),(int)((wy-pos.y)*multiplier+g.scHeight/2-width/2.0*multiplier),(int)(width*multiplier),(int)(width*multiplier),nullptr,angle);
}

//Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
void Camera::renderTile(Texture &arg,double wx,double wy,double width) const {
    int pixelx=(int)floor((wx-pos.x)*width*multiplier+g.scWidth/2);
    int pixely=(int)floor((wy-pos.y)*width*multiplier+g.scHeight/2);
    int pixelrx=(int)ceil((wx-pos.x+width)*width*multiplier+g.scWidth/2);
    int pixeldy=(int)ceil((wy-pos.y+width)*width*multiplier+g.scHeight/2);

    arg.renderScaled(pixelx,pixely,pixelrx-pixelx,pixeldy-pixely);
}

Vector2 Camera::getPos() { return pos; }

void Camera::setPos(Vector2 x) { pos=x; }

double Camera::getMultiplier() { return multiplier; }

void Camera::setMultiplier(double arg) { multiplier=arg; }



//Default implementations for CameraController.
CameraController::CameraController() : camP(nullptr) { }

//returns cameraPointer!=nullptr
bool CameraController::cameraValid() {
    return camP!=nullptr;
}

//sets the internal camera pointer to the argument
void CameraController::attachCamera(Camera *cameraPointer) { 
    detachCamera(); 
    camP=cameraPointer; 
}

//sets cameraPointer to null. Also zeroes internal state like camera velocity.
void CameraController::detachCamera() {
    camP=nullptr;
}

//Update internal state (velocity, friction, etc.). Default implementation does nothing.
void CameraController::timestep(double dt) { }



/*


    //camera velocity
    Vector2 v;

    //exponential drag coefficient.
    double drag;

    //max velocity
    double maxv;

    //how fast to pan towards the character if it is offscreen
    double panv;

    //true if our current goal is to center the character onscreen.
    bool goalIsToCenter;



    Vector2 targetWorldPosition;
    int targetRectangleW;
    int targetRectangleH;
    double targetBandStrength;

    Vector2 mousePixelPosition;
    int mouseRectangleW;
    int mouseRectangleH;
    double mouseBandStrength;
    */
    //resets v and goalIsToCenter.
FollowerCameraController::FollowerCameraController() : v(),drag(5),maxv(100),panv(20),panacc(100), goalIsToCenter(false),
    targetWorldPosition(),targetRectangleW(0),targetRectangleH(0),targetBandStrength(100),
    mousePixelPosition(),mouseRectangleW(0),mouseRectangleH(0),mouseBandStrength(50) { 
    targetRectangleW=(int)((2.0/3.0-0.05)*g.scWidth);
    targetRectangleH=(int)((2.0/3.0-0.05)*g.scHeight);
    mouseRectangleW=(int)((2.0/3.0)*g.scWidth);
    mouseRectangleH=(int)((2.0/3.0)*g.scHeight);
}
void FollowerCameraController::resetState() {
    v=Vector2();
    goalIsToCenter=false;
}

bool FollowerCameraController::mouseInRectangle() {
    int l=g.scWidth/2-mouseRectangleW/2;
    int r=g.scWidth/2+mouseRectangleW/2;
    int u=g.scHeight/2-mouseRectangleH/2;
    int d=g.scHeight/2+mouseRectangleH/2;
    int x=(int)mousePixelPosition.x;
    int y=(int)mousePixelPosition.y;
    return (x>=l && x<r && y>=u && y<d);

}
bool FollowerCameraController::targetInRectangle() {
    if (!cameraValid())
        return false;
    Vector2 targetPixelPosition=camP->worldToPixels(targetWorldPosition);

    int l=g.scWidth/2-targetRectangleW/2;
    int r=g.scWidth/2+targetRectangleW/2;
    int u=g.scHeight/2-targetRectangleH/2;
    int d=g.scHeight/2+targetRectangleH/2;
    int x=(int)targetPixelPosition.x;
    int y=(int)targetPixelPosition.y;
    return (x>=l && x<r && y>=u && y<d);
}

void FollowerCameraController::setTargetPosition(Vector2 pt) {
    targetWorldPosition=pt;
}

//immediately center the target.
void FollowerCameraController::forceCenterTarget() {
    if (!cameraValid())
        return;
    camP->setPos(targetWorldPosition);
}

//transition/pan over time so that the target is centered.
void FollowerCameraController::centerTarget() {
    goalIsToCenter=true;
}

//sets cameraPointer to null. Also zeroes internal state like camera velocity.
void FollowerCameraController::detachCamera() {
    CameraController::detachCamera();
    resetState();
}

//Handle dragging mouse input.
bool FollowerCameraController::handleEvent(SDL_Event *e) {
    return false;
}

//Update internal state (velocity, friction, etc.)
void FollowerCameraController::timestep(double dt) {
    mousePixelPosition=Vector2(g.mousex,g.mousey);
    if (!cameraValid())
        return;
    if (!goalIsToCenter) {
        Vector2 force;

        //define "screen vectors" to be 0 at the center of the screen, "(-1,0)" at the left of the screen, and
        //"(1,0)" at the right of the screen.

        Vector2 mouseScreenVector=Vector2();
        if (!mouseInRectangle()) {
            mouseScreenVector=(mousePixelPosition-Vector2(g.scWidth/2.0,g.scHeight/2.0))/((double)g.scWidth/2.0);
        }

        Vector2 targetScreenVector=Vector2();
        //if (!targetInRectangle()) {
            Vector2 targetPixelPosition=camP->worldToPixels(targetWorldPosition);
            targetScreenVector=(targetPixelPosition-Vector2(g.scWidth/2.0,g.scHeight/2.0))/((double)g.scWidth/2.0);
        //}

        force=targetScreenVector*targetBandStrength+mouseScreenVector*mouseBandStrength;

        camP->setPos(camP->getPos()+v*dt);
        v=v+force*dt;
        v*=exp(-dt*drag);
        
    }
    else {
        //goal is to center, set velocity!
        Vector2 diff=(targetWorldPosition-camP->getPos());
        double d=diff.length();
        double vmag=v.length();
        double cutoffdist=fmax(vmag/drag,1);

        if (d<cutoffdist) {
            goalIsToCenter=false;
            camP->setPos(camP->getPos()+v*dt);
        }
        else {
            if (vmag-0.001<panv) {
                //if we haven't reached max speed yet, accelerate.
                vmag+=panacc*dt;
                //if we've just gone over, set equal to panv.
                if (vmag>panv)
                    vmag=panv;
            }
            else {
                vmag=panv;
            }
            v=diff*vmag/d;
            camP->setPos(camP->getPos()+v*dt);
        }
    }
}












DraggingCameraController::DraggingCameraController() : CameraController(), cameradraggable(false),v(),drag(10),maxv(200),mousedragpos(),mousedragpospixels(), lastpos(),dragging(false),dragbutton(-1),lastdt(0.01) { }

//resets v, mousedragpos,mousedragpospixels,dragging, and lastdt.
void DraggingCameraController::resetState() {
    v=Vector2();
    mousedragpos=Vector2();
    mousedragpospixels=Vector2();
    lastpos=Vector2();
    dragging=false;
    lastdt=0.01;
}

void DraggingCameraController::detachCamera() {
    CameraController::detachCamera();
    resetState();
}

void DraggingCameraController::setDraggable(bool arg) {
    cameradraggable=arg;
    if (!cameradraggable) 
        resetState();
}


void DraggingCameraController::setDragButton(int arg) {
    if (arg!=dragbutton) 
        resetState();
    dragbutton=arg;
}

bool DraggingCameraController::getCameraDraggable() {
    return cameradraggable;
}

//Core things: events & movement!
bool DraggingCameraController::handleEvent(SDL_Event *e) {
    //ensure camP is non-null.
    if (!cameraValid())
        return false;

    bool buttonclicked= (e->type==SDL_MOUSEBUTTONDOWN);
    bool buttonreleased=(e->type==SDL_MOUSEBUTTONUP);
    bool buttonmatches = false;
    if (buttonclicked||buttonreleased) {
        if (dragbutton==-1)
            buttonmatches=true;
        else if (dragbutton==0) {
            buttonmatches=(e->button.button==SDL_BUTTON_LEFT);
        }
        else if (dragbutton==1) {
            buttonmatches=(e->button.button==SDL_BUTTON_RIGHT);
        }
        else if (dragbutton==2) {
            buttonmatches=(e->button.button==SDL_BUTTON_MIDDLE);
        }
    }


    if (e->type == SDL_MOUSEBUTTONDOWN && buttonmatches && cameradraggable) {
        dragging=true;
        mousedragpos=camP->getPos();
        mousedragpospixels=Vector2(g.mousex,g.mousey);
        lastpos=camP->getPos();
        return true;
    }
    else if (dragging && e->type == SDL_MOUSEBUTTONUP && buttonmatches && cameradraggable) {
        v=(camP->getPos()-lastpos)/lastdt;

        dragging=false;
        mousedragpos=Vector2();
        mousedragpospixels=Vector2();
        lastpos=Vector2();
    }
    return false;
}

void DraggingCameraController::timestep(double dt) {
    //ensure camP is non-null.
    if (!cameraValid())
        return;

    lastdt=dt;
    if (v.length()>maxv)
        v=v.normalized()*maxv;
    v*=fmax((1-dt*drag),0);
    if (!dragging && cameradraggable) {
        camP->setPos(camP->getPos()+dt*v);
    }
    else if (dragging && cameradraggable) {
        lastpos=camP->getPos();
        camP->setPos((mousedragpospixels-Vector2(g.mousex,g.mousey))/camP->getMultiplier()+mousedragpos);
    }
}

