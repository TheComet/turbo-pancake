#include "GSMapEditor.h"



using namespace std;
GSMapEditor::GSMapEditor() : back(), toggleBackground(),togglers(), editorx(0),editory(0), editorpaint(0), stateChange(0),leftclickheld(false),rightclickheld(false),xtilelast(0),ytilelast(0) { }

void GSMapEditor::initialize() {
    //Initialize and position the main screen buttons
    Texture p=loadTexture("media/backbutton_pressed.png");
    Texture up=loadTexture("media/backbutton.png");
    back=Button(g.scWidth -10 - p.getWidth(),10,p,up);

    toggleBackground=loadTexture("media/editorpanel.png");
    toggleBackground.setAlpha(125);

    editorx=10;
    editory=g.scHeight-toggleBackground.getHeight()-10;
    int dx=(toggleBackground.getWidth()-30)/4;
    int dx0=15;
    int yshift=-4;


    Texture tp=loadTexture("media/grey_button09.png");
    Texture tup=loadTexture("media/grey_button08.png");

    Texture t1=loadTexture("media/tileicon.png");
    Texture t2=loadTexture("media/wallicon.png");
    Texture t3=loadTexture("media/team1spawnicon.png");
    Texture t4=loadTexture("media/team2spawnicon.png");
    togglers.push_back(ToggleButton(editorx+dx0,editory+toggleBackground.getHeight()/2-t1.getHeight()/2+yshift,
        tp,tup,"",t1));
    togglers.push_back(ToggleButton(editorx+dx0+dx,editory+toggleBackground.getHeight()/2-t2.getHeight()/2+yshift,
        tp,tup,"",t2));
    togglers.push_back(ToggleButton(editorx+dx0+2*dx,editory+toggleBackground.getHeight()/2-t3.getHeight()/2+yshift,
        tp,tup,"",t3));
    togglers.push_back(ToggleButton(editorx+dx0+3*dx,editory+toggleBackground.getHeight()/2-t4.getHeight()/2+yshift,
        tp,tup,"",t4));
    togglers[0].setState(true);
    togglers[1].setState(false);
    togglers[2].setState(false);
    togglers[3].setState(false);


    stateChange=0;
    map.initialize();
    cam.setDraggable();
    cam.setDragButton(2);
}


void GSMapEditor::updateEditorPaint() {
    for (size_t i=0;i<togglers.size();i++) {
        if (togglers[i].wasUpdated()) {
            if (togglers[i].getState()) {
                for (size_t j=0;j<togglers.size();j++) {
                    if (i!=j) {
                        togglers[j].setState(false);
                        togglers[j].updateReceived();
                    }
                }
                editorpaint=i;
            }
            else {
                editorpaint=-1;
            }
            cout<<"True!"<<endl;

            togglers[i].updateReceived();
            break;
        }
    }
}

int GSMapEditor::getStateChange() { return stateChange;  }
void GSMapEditor::resetStateChange() { stateChange=0;  }
//Timestep. Calls timestep on the current active game state.
void GSMapEditor::timestep(double dt) {
    if (back.isPressed()) {
        stateChange=1;
        back.pressReceived();
    }
    updateEditorPaint();
    cam.timestep(dt);
    if (leftclickheld) {
        Vector2 w=cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        int xtilenew=(int)w.x;
        int ytilenew=(int)w.y;
        if (xtilenew!=xtilelast || ytilenew!=ytilelast) {
            if (editorpaint==0)
                map.setTile(xtilenew,ytilenew,Tiles::dirt);
            else if (editorpaint==1)
                map.addWall(xtilenew,ytilenew);
            else if (editorpaint==2)
                map.setSpawn(xtilenew,ytilenew,0);
            else if (editorpaint==3)
                map.setSpawn(xtilenew,ytilenew,1);
        }
        xtilelast=xtilenew;
        ytilelast=ytilenew;
    } else if(rightclickheld) {
        Vector2 w=cam.pixelsToWorld(Vector2(g.mousex,g.mousey));
        int xtilenew=(int)w.x;
        int ytilenew=(int)w.y;
        if (xtilenew!=xtilelast || ytilenew!=ytilelast) {
            if (editorpaint==0)
                map.setTile(xtilenew,ytilenew,Tiles::empty);
            else if (editorpaint==1)
                map.eraseWall(xtilenew,ytilenew);
            else if (editorpaint==2)
                map.setSpawn(xtilenew,ytilenew,-1);
            else if (editorpaint==3)
                map.setSpawn(xtilenew,ytilenew,-1);
        }
        xtilelast=xtilenew;
        ytilelast=ytilenew;
    }
}
void GSMapEditor::render() {
    map.render(cam);
    map.renderSpawns(cam);

    back.render();

    toggleBackground.render(editorx,editory);
    for (size_t i=0;i<togglers.size();i++)
        togglers[i].render();

}
void GSMapEditor::handleEvent(SDL_Event *e) {
    g.mouseCapturedByGUI=(g.mouseCapturedByGUI||back.handleEvent(e)||cam.handleEvent(e));

    for (size_t i=0;i<togglers.size();i++)
        g.mouseCapturedByGUI=g.mouseCapturedByGUI||togglers[i].handleEvent(e);

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