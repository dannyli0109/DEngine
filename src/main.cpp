#include "renderer/window.h"
#include <iostream>

int main(void)
{
    DEngine::initGlfw();
    DEngine::Window *window = DEngine::createWindow(1280, 720, "Hello World");
    // Setup ImGui context
    DEngine::initImGui(window);
    // Load all OpenGL function pointers using GLAD
    DEngine::initGlad();

    /* Loop until the user closes the window */
    while (!DEngine::shouldCloseWindow(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        // End of ImGui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window->glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }
    DEngine::destroyWindow(window);
    glfwTerminate();
    return 0;
}