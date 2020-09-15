//
// Created by jon on 13/09/2020.
//

#include "Texture2D.h"
#include "../third_party/stb_image.h"
#include <stdexcept>
#include <iostream>

Texture2D::Texture2D(const std::string& file_path) {
    glGenTextures(1, &gl_texture_id);
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    if(unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0)) {
        // TODO: Account for other nrChannels values
        const GLenum channels = nrChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        throw std::runtime_error("Failed to load image " + file_path);
    }
}


void Texture2D::bind(GLenum texture_unit) const {
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
}
