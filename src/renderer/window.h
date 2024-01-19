// C++
#include <GLFW/glfw3.h>
#include <string>
#include <glm.hpp>

namespace DEngine
{
    class Window
    {
    public:
        GLFWwindow *window;
        unsigned int width = 800;
        unsigned int height = 300;
        std::string title = "";

        Window(int width, int height, std::string title)
            : width(width), height(height), title(title)
        {
            // Initialize and configure GLFW
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            // Create a windowed mode window and its OpenGL context
            window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
            if (!window)
            {
                glfwTerminate();
                return;
            }
            beginWindow();

            glfwSetWindowPos(window, 0, 30);

            glfwSwapInterval(1);

            glfwSetWindowUserPointer(window, this);
        }

        bool shouldClose() const
        {
            return glfwWindowShouldClose(window);
        }

        void swapBuffers()
        {
            glfwSwapBuffers(window);
        }

        void pollEvents()
        {
            glfwPollEvents();
        }

        glm::vec2 getSize()
        {
            return glm::vec2(width, height);
        }

        void beginWindow()
        {
            glfwMakeContextCurrent(window);
        }

        ~Window()
        {
            glfwTerminate();
        }
    };
}