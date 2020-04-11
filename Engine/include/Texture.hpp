#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>

class Texture
{
private:
    unsigned int id;
    unsigned int texture_slot;
    std::string file_path;
    unsigned char* local_buffer;
    int width, height, bits_per_pixel;

public:
    Texture(const std::string& path);
    Texture(bool shadow_map, unsigned int* depth_frame_buffer, unsigned int shadow_width, unsigned int shadow_height);
    ~Texture();

    void Bind(unsigned int slot);
    void Unbind();

    inline int GetWidth() const { return width;}
    inline int GetHeight() const { return height;}
    inline int GetBitsPerPixel() const { return bits_per_pixel;}
    inline int GetTextureSlot() const { return texture_slot;}
};

#endif /* _TEXTURE_H */