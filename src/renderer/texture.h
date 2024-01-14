#include <string>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DEngine
{
    class Texture
    {
    public:
        Texture(std::string filename)
        {
            int width;
            int height;
            int channels;

            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 4);
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                loadedSuccessfully = true;
                std::cout << "Loaded " << filename << id << std::endl;
            }
            else
            {
                std::cout << "Fail to load texture" << std::endl;
                loadedSuccessfully = false;
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, 0);

            if (!loadedSuccessfully)
            {
                glDeleteTextures(1, &id);
            }

            stbi_image_free(data);
        }

#pragma region Rule of Five
        ~Texture()
        {
            glDeleteTextures(1, &id);
        }
        Texture(const Texture &other) = delete;
        Texture &operator=(const Texture &other) = delete;
        Texture(const Texture &&other) = delete;
        Texture &operator=(const Texture &&other) = delete;
#pragma endregion

        void bind(int textureUnit)
        {
            // std::cout << "Binding texture " << id << " to texture unit " << GL_TEXTURE0 + textureUnit << std::endl;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, id);
        }
        void destroy()
        {
            if (loadedSuccessfully)
            {
                glDeleteTextures(1, &id);
                loadedSuccessfully = false;
            }
        }

        static void unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        bool operator==(const Texture &other)
        {
            return id == other.id;
        }

        GLuint getID()
        {
            return id;
        }

    private:
        GLuint id;
        bool loadedSuccessfully;
    };

} // namespace DEngine