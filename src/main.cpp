#include "editor/editor.h"
#include "renderer/renderer.h"
#include "renderer/resourceManager.h"
#include "renderer/camera.h"
int main()
{
    DEngine::Editor editor;
    DEngine::ResourceManager *resourceManager = DEngine::ResourceManager::getInstance();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    resourceManager->addTexture(new DEngine::Texture("./working/frame0.png"));
    resourceManager->addTexture(new DEngine::Texture("./working/wall.jpg"));

    DEngine::Shader *quadShader = new DEngine::Shader("./working/quad.vs", "./working/quad.fs");

    DEngine::QuadRenderer *quadRenderer = new DEngine::QuadRenderer(quadShader, 1024);
    glm::vec2 windowSize = editor.window.getSize();
    DEngine::Camera2D *camera = new DEngine::Camera2D({0, 0}, 4.0f, windowSize);
    editor.setRenderLogic(
        [&]()
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            quadRenderer->begin(camera);
            quadRenderer->drawQuad(glm::vec3(0, 0, 0), resourceManager->getTexture(0));
            quadRenderer->drawQuad(glm::vec3(1, 0, 0), resourceManager->getTexture(1));
            quadRenderer->end();

            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            ImGui::End();
        });
    editor.mainLoop();

    return 0;
}
