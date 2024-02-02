#include "renderer/window.h"
#include "renderer/imgui_container.h"
#include <iostream>
#include "renderer/shader.h"
#include "renderer/renderer.h"
#include "renderer/texture.h"
#include "renderer/camera.h"

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
    DEngine::QuadRenderer *renderer = DEngine::createQuadRenderer(shader, 1024);

    // Create texture
    DEngine::Texture *texture = DEngine::createTexture("./working/frame0.png");

    // Create camera
    DEngine::Camera *camera = DEngine::createCamera2D({0, 0}, 0, {1280, 720});

    // Create lights
    std::vector<DEngine::Light> lights;

    for (int i = 0; i < 2; i++)
    {
        lights.push_back({{i * 10 - 5, 0, 0},
                          {1, 1, 1, 1},
                          1.0f});
    };

    /* Loop until the user closes the window */
    while (!DEngine::shouldCloseWindow(window))
    {
        // Start of ImGui rendering
        DEngine::Imgui::beginImgui();
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Start of rendering
        // DEngine::beginQuadRenderer(renderer, camera);
        // DEngine::addLights(renderer, lights);
        // DEngine::drawQuad(renderer, {0, 0, 0}, {1, 1, 1}, texture);
        // DEngine::endQuadRenderer(renderer);

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