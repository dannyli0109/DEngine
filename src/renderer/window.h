#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>

namespace DEngine
{
    typedef struct Window
    {
        unsigned int width;
        unsigned int height;
        const char *title;
        GLFWwindow *glfwWindow;
    } Window;

    Window *createWindow(unsigned int width, unsigned int height, const char *title)
    {
        Window *window = (Window *)malloc(sizeof(Window));
        window->width = width;
        window->height = height;
        window->title = title;
        window->glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!window->glfwWindow)
        {
            glfwTerminate();
            return NULL;
        }
        glfwMakeContextCurrent(window->glfwWindow);
        return window;
    }

    void destroyWindow(Window *window)
    {
        glfwDestroyWindow(window->glfwWindow);
        free(window);
    }

    void initGlfw()
    {
        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    void initGlad()
    {
        // Load all OpenGL function pointers using GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }
    }

    bool shouldCloseWindow(Window *window)
    {
        return glfwWindowShouldClose(window->glfwWindow);
    }
}