#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <functional>
#include "../renderer/window.h"

namespace DEngine
{
    class Editor
    {
    public:
        using RenderFunction = std::function<void()>;
        Window window = Window(800, 600, "Your Game Engine");

        Editor()
        {
            init();
            initImGui();
        }

        ~Editor()
        {
            shutdownImGui();
            glfwTerminate();
        }

        void init()
        {
            // Load all OpenGL function pointers using GLAD
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        void initImGui()
        {
            // Setup ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(window.window, true);
            ImGui_ImplOpenGL3_Init("#version 330");
        }

        void mainLoop()
        {
            while (!window.shouldClose())
            {
                // External ImGui rendering logic is passed here
                updateImGui(externalRenderLogic);

                window.swapBuffers();
                window.pollEvents();
            }
        }

        void updateImGui(RenderFunction externalRenderLogic)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

            // External ImGui logic execution
            if (externalRenderLogic)
            {
                externalRenderLogic();
            }

            // End of ImGui rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            updateImGuiViewports();
        }

        void setRenderLogic(RenderFunction newRenderLogic)
        {
            externalRenderLogic = newRenderLogic;
        }

        void shutdownImGui()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }

    private:
        RenderFunction externalRenderLogic;
        void updateImGuiViewports()
        {
            ImGuiIO &io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
    };
}
