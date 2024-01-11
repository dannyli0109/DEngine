#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <functional>

namespace DEngine
{
    class Editor
    {
    public:
        using ImGuiRenderFunction = std::function<void()>;

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
            // Initialize and configure GLFW
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            // Create a window object
            window = glfwCreateWindow(800, 600, "Your Game Engine", NULL, NULL);
            if (window == NULL)
            {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, Editor::framebufferSizeCallback);

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
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 330");
        }

        void mainLoop()
        {
            while (!glfwWindowShouldClose(window))
            {
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // External ImGui rendering logic is passed here
                updateImGui(externalImGuiRenderLogic);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
        }

        void updateImGui(ImGuiRenderFunction externalRenderLogic)
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

        void setImGuiRenderLogic(ImGuiRenderFunction newRenderLogic)
        {
            externalImGuiRenderLogic = newRenderLogic;
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
        GLFWwindow *window;
        ImGuiRenderFunction externalImGuiRenderLogic;

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
