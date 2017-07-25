#include "Camera.h"
#include <cmath>  


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

