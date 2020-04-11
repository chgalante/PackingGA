
#include <GL/glew.h>
#include "../../Dependencies/include/stb_image/stb_image.h"
#include "../include/Texture.hpp"

Texture::Texture(const std::string& path)
    : id(0), file_path(path), local_buffer(nullptr), width(0), height(0), bits_per_pixel(0)
{
    stbi_set_flip_vertically_on_load(1);
    local_buffer = stbi_load(path.c_str(), &width, &height, &bits_per_pixel, 4);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(local_buffer)
         stbi_image_free(local_buffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int slot)
{
    texture_slot = slot;
    glActiveTexture(GL_TEXTURE0 + texture_slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind()
{
    glActiveTexture(GL_TEXTURE0 + texture_slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}