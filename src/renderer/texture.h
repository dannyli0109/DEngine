#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "graphics_include.h"

namespace DEngine
{
    typedef struct Texture
    {
        unsigned int id;
    } Texture;

    Texture *createTexture(const char *path)
    {
        int width, height, channels;
        Texture *texture = (Texture *)malloc(sizeof(Texture));
        unsigned char *data = stbi_load(path, &width, &height, &channels, 4);
        if (!data)
        {
            std::cout << "Failed to load texture" << std::endl;
            free(texture);
            return NULL;
        }

        glGenTextures(1, &texture->id);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        return texture;
    }

    void destroyTexture(Texture *texture)
    {
        glDeleteTextures(1, &texture->id);
        free(texture);
    }

    void bindTexture(Texture *texture, int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    void unbindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}