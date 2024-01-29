#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

    void initGlfw()
    {
        if (!glfwInit())
            return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void initImGui(Window *window)
    {
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window->glfwWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330");
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