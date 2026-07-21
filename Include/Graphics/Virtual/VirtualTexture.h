//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_VIRTUALTEXTURE_H
#define EUROPARENDER_VIRTUALTEXTURE_H

class VirtualTexture {
private:
    GLuint textureID;
    bool isBinded = false;
public:
    virtual bool IsBinded();
    virtual void BindTexture();
    virtual void RenderTexture();
    virtual void SetData(std::vector<unsigned char> data,int width,int height,GLenum type,GLenum format,GLenum internalFormat);
    virtual GLuint GetTextureID();
protected:
    std::vector<unsigned char> textureData;
    int width, height, channels;
    GLenum type;GLenum format;GLenum internalFormat;
};

#endif //EUROPARENDER_VIRTUALTEXTURE_H
