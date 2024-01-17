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
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 300;
        std::string title = "Your Game Engine";

        Window(int width, int height, std::string title)
            : SCR_WIDTH(width), SCR_HEIGHT(height), title(title)
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
            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);
            if (!window)
            {
                glfwTerminate();
                return;
            }
            beginWindow();
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
            return glm::vec2(SCR_WIDTH, SCR_HEIGHT);
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