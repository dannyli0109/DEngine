#include "renderer/window.h"
#include "renderer/imgui_container.h"
#include <iostream>
#include "renderer/shader.h"

int main(void)
{
    DEngine::initGlfw();
    DEngine::Window *window = DEngine::createWindow(1280, 720, "Hello World");
    // Setup ImGui context
    DEngine::Imgui::initImgui(window);
    // Load all OpenGL function pointers using GLAD
    DEngine::initGlad();

    // Create shader
    DEngine::Shader *shader = DEngine::createShader("./working/quad.vs", "./working/quad.fs");

    /* Loop until the user closes the window */
    while (!DEngine::shouldCloseWindow(window))
    {
        // Start of ImGui rendering
        DEngine::Imgui::beginImgui();
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        // End of ImGui rendering
        DEngine::Imgui::endImgui();

        /* Swap front and back buffers */
        glfwSwapBuffers(window->glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }
    DEngine::destroyWindow(window);
    glfwTerminate();
    return 0;
}