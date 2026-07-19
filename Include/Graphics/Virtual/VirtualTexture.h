//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_VIRTUALTEXTURE_H
#define EUROPARENDER_VIRTUALTEXTURE_H

class VirtualTexture {
private:
    GLuint textureID;
public:
    virtual void BindTexture();
    virtual GLuint GetTextureID();
protected:
    unsigned char *textureData;
    int width, height, channels;
};

#endif //EUROPARENDER_VIRTUALTEXTURE_H
