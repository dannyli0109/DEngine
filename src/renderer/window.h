#include <GLFW/glfw3.h>
#include <stdlib.h>

namespace DEngine
{
    typedef struct Window
    {
        unsigned int width;
        unsigned int height;
        const char *title;
        GLFWwindow *glfwWindow;
    } Window;

    typedef struct ImguiContainer
    {
        ImGuiIO *io;
        ImGuiContext *context;
    } ImguiContainer;

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

    initGlfw()
    {
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
}