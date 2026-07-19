//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_STBGRAPHICSTEXTURE_H
#define EUROPARENDER_STBGRAPHICSTEXTURE_H

#include "pch.h"

#include "VirtualTexture.h"
#include <string>

class StbGraphicsTexture:public VirtualTexture {
public:
    StbGraphicsTexture();
    void SetTexture(std::string path);
    ~StbGraphicsTexture();

    void BindTexture() override;
private:
    GLuint textureID;
};

#endif //EUROPARENDER_STBGRAPHICSTEXTURE_H
