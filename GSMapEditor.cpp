#include "GSMapEditor.h"



GSMapEditor::GSMapEditor() : back(),stateChange(0),leftclickheld(false),rightclickheld(false),xtilelast(0),ytilelast(0) { }

void GSMapEditor::initialize() {
    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/backbutton_pressed.png");
    Texture up=loadTexture("media/backbutton.png");
    back=Button(g.scWidth -10 - p.getWidth(),10,p,up);

    stateChange=0;
    map.initialize();
    cam.setDraggable();
    cam.setDragButton(2);
}

int GSMapEditor::getStateChange() { return stateChange;  }
void GSMapEditor::resetStateChange() { stateChange=0;  }
//Timestep. Calls timestep on the current active game state.
void GSMapEditor::timestep(double dt) {
    if (back.isPressed()) {
        stateChange=1;
        back.pressReceived();
    }
    cam.timestep(dt);
    if (leftclickheld) {
        Vector2 w=cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        int xtilenew=(int)w.x;
        int ytilenew=(int)w.y;
        if (xtilenew!=xtilelast || ytilenew!=ytilelast) {
            map.addWall(xtilenew,ytilenew);
        }
        xtilelast=xtilenew;
        ytilelast=ytilenew;
    } else if (rightclickheld) {
        Vector2 w=cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        int xtilenew=(int)w.x;
        int ytilenew=(int)w.y;
        if (xtilenew!=xtilelast || ytilenew!=ytilelast) {
            map.eraseWall(xtilenew,ytilenew);
        }
        xtilelast=xtilenew;
        ytilelast=ytilenew;
    }
}
void GSMapEditor::render() {
    map.render(cam);
    back.render();
}
void GSMapEditor::handleEvent(SDL_Event *e) {
    g.mouseCapturedByGUI=(g.mouseCapturedByGUI||back.handleEvent(e)||cam.handleEvent(e));
    if (!g.mouseCapturedByGUI) {
        if (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_LEFT) {
            leftclickheld=true;
            Vector2 w=cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
            xtilelast=-1;
            ytilelast=-1;
        }
        else if (e->type==SDL_MOUSEBUTTONDOWN && e->button.button==SDL_BUTTON_RIGHT) {
            rightclickheld=true;
            Vector2 w=cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
            xtilelast=-1;
            ytilelast=-1;

        } else if (e->type==SDL_MOUSEBUTTONUP && e->button.button==SDL_BUTTON_LEFT) {
            leftclickheld=false;
            xtilelast=-1;
            ytilelast=-1;
        }
        else if (e->type==SDL_MOUSEBUTTONUP && e->button.button==SDL_BUTTON_RIGHT) {
            rightclickheld=false;
            xtilelast=-1;
            ytilelast=-1;
        }
    }
}

//window resized event callback
void GSMapEditor::windowResized() {
    back.setPos(g.scWidth -10 - back.getUnpressedTexture().getWidth(),10);
}