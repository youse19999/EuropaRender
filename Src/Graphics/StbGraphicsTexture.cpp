//
// Created by youse on 2026/07/19.
//

#include <iostream>
#include <ostream>
#include <Graphics/StbGraphicsTexture.h>

#include "logger.h"
#include "stb_image.h"

void StbGraphicsTexture::SetTexture(std::string path) {
    this->textureData = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (this->textureData == NULL) {
        std::cerr << "Error loading stb image: " << path << std::endl;
        return;
    }
}

StbGraphicsTexture::~StbGraphicsTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureID);
    stbi_image_free(textureData);
}

StbGraphicsTexture::StbGraphicsTexture() {

}

void StbGraphicsTexture::BindTexture() {
    glGenTextures(1, &textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (this->textureData == nullptr) {
        std::cout << "StbGraphicsTexture::BindTexture(): textureData is nullptr" << std::endl;
    }
    if (this->textureData == nullptr) {
        LOG_RENDER("TEXTURE DATA IS NULL");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->textureData);
}