#include "graphicsInclude.h"
#include <iostream>

namespace DEngine
{
    class Framebuffer
    {
    private:
        GLuint fbo;        // Framebuffer object
        GLuint texture;    // Texture attached to framebuffer
        int width, height; // Dimensions of the framebuffer
        bool initialised = false;

    public:
        Framebuffer(int width, int height) : width(width), height(height)
        {
            create(width, height);
        }

        void create(int width, int height)
        {
            destroy();
            initialised = true;
            glGenFramebuffers(1, &fbo);
            glGenTextures(1, &texture);

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            {
                std::cout << "Frame Buffer initialised successfully!" << std::endl;
            }
            else
            {
                std::cout << "Frame Buffer failed to initialise" << std::endl;
                destroy();
            }
        }

        void destroy()
        {
            if (initialised)
            {
                glDeleteFramebuffers(1, &fbo);
                glDeleteTextures(1, &texture);

                initialised = false;
            }
        }

        ~Framebuffer()
        {
            glDeleteFramebuffers(1, &fbo);
            glDeleteTextures(1, &texture);
        }

        void bind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        }

        static void unbind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void bindTexture()
        {
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        GLuint getTextureID() const
        {
            return texture;
        }

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }
    };
};
