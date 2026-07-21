//
// Created by youse on 2026/07/19.
//
#include <iostream>
#include <random>
#include <VirtualTexture.h>

#include "GameWindow.h"
#include "logger.h"

void VirtualTexture::BindTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, this->textureData.data());
    isBinded = true;
}

bool VirtualTexture::IsBinded() {
    return isBinded;
}

void VirtualTexture::RenderTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0);
}
void VirtualTexture::SetData(std::vector<unsigned char> data,int width,int height,GLenum type,GLenum format,GLenum internalFormat) {
    this->textureData = data;
    this->width = width;
    this->height = height;
    this->channels = 3;
    this->type = type;
    this->format = format;
    this->internalFormat = internalFormat;
}

GLuint VirtualTexture::GetTextureID() {
    return textureID;
}