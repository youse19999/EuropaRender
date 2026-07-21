//
// Created by youse on 2026/07/19.
//

#include <iostream>
#include <ostream>
#include <Graphics/StbGraphicsTexture.h>
#include <pch.h>
#include "logger.h"

void StbGraphicsTexture::SetTexture(std::string path) {
    //this->textureData = std::vector<unsigned int>(stbi_load(path.c_str(), &width, &height, &channels, 0));
    /*if (this->textureData == NULL) {
        std::cerr << "Error loading stb image: " << path << std::endl;
        return;
    }*/
}

StbGraphicsTexture::~StbGraphicsTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureID);
    stbi_image_free(textureData.data());
}

StbGraphicsTexture::StbGraphicsTexture() {

}