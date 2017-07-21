#include "Camera.h"
#include <cmath>  


Camera::Camera () : pos(), multiplier(50),cameradraggable(false),v(),drag(10),maxv(200),mousedragpos(),lastpos(),dragging(false),lastdt(0.01) { }

//Ensures that the point at (x,y) is at the center of the screen.
//Note that the center of the tile at (0,0) has coordinates (0.5,0.5).
void Camera::centerCameraOnTile(double x,double y) {
    pos=Vector2(x,y);
}

void Camera::toggleCameraDraggable() {
    cameradraggable=!cameradraggable;
    if (!cameradraggable) {
        dragging=false;
        mousedragpos=mousedragpospixels=lastpos=Vector2();
    }

}
void Camera::setDraggable() { cameradraggable=true;  }
bool Camera::getCameraDraggable() {
    return cameradraggable;
}

//Core things: events & movement!
bool Camera::handleEvent(SDL_Event *e) {
    if (e->type == SDL_MOUSEBUTTONDOWN && cameradraggable) {
        dragging=true;
        mousedragpos=pos;
        mousedragpospixels=Vector2(g.mousex,g.mousey);
        lastpos=pos;
        return true;
    } else if (dragging && e->type == SDL_MOUSEBUTTONUP && cameradraggable) {
        v=(pos-lastpos)/lastdt;

        dragging=false;
        mousedragpos=Vector2();
        mousedragpospixels=Vector2();
        lastpos=Vector2();
    }
    return false;
}
void Camera::timestep(double dt) {
    lastdt=dt;
    if (v.length()>maxv)
        v=v.normalized()*maxv;
    v*=fmax((1-dt*drag),0);
    if (!dragging && cameradraggable) {
        pos+=dt*v;
    } else if (dragging && cameradraggable){
        lastpos=pos;
        pos=(mousedragpospixels-Vector2(g.mousex,g.mousey))/multiplier+mousedragpos;
    }
}

//Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
void Camera::renderTexture(Texture &arg,double wx,double wy,double angle,double width) const {
    arg.renderScaled((int)((wx-pos.x)*width*multiplier+g.scWidth/2),(int)((wy-pos.y)*width*multiplier+g.scHeight/2),(int)(width*multiplier),(int)(width*multiplier),nullptr,angle);
}
//Render texture on the screen. wx and wy are in tile coordinates. width is in terms of tiles.
void Camera::renderTile(Texture &arg,double wx,double wy,double width) const {
    int pixelx=(int)floor((wx-pos.x)*width*multiplier+g.scWidth/2);
    int pixely=(int)floor((wy-pos.y)*width*multiplier+g.scHeight/2);
    int pixelrx=(int)ceil((wx-pos.x+width)*width*multiplier+g.scWidth/2);
    int pixeldy=(int)ceil((wy-pos.y+width)*width*multiplier+g.scHeight/2);

    arg.renderScaled(pixelx,pixely,pixelrx-pixelx,pixeldy-pixely);
}
