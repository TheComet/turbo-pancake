#pragma once

#include "Texture.h"
#include "Globals.h"
#include "Camera.h"
#include <vector>

class Stickers {
    struct sticker {
        Texture tex;
        Vector2 pos;
        double angle;
        double width;
    };
    std::vector<sticker> list;

public:
    Stickers() : list() { }
    void addSticker(Texture tex, Vector2 pos, double angle, double width) {
        list.push_back({tex,pos,angle, width});
    }
    void render(const Camera& arg) {
        for (auto i=list.begin();i!=list.end();i++)
            arg.renderTexture((*i).tex,(*i).pos.x,(*i).pos.y,(*i).angle,(*i).width);
    }
    void clearStickers() {
        list.clear();
    }
};