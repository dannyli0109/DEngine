#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    void run();

private:
    GLFWwindow* window;

    void initialize();
    void initializeImGui();
    void beginImGuiFrame();
    void endImGuiFrame();
    void renderImgui();
    void processInput();
    void update();
    void render();
    void cleanup();
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif // GAME_ENGINE_H