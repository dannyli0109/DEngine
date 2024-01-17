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
    glm::vec3 shadowOffset = glm::vec3(0.1f, -0.3f, 0); // adjust as needed
    glm::vec4 shadowColor = glm::vec4(0, 0, 0, 0.5f);   // adjust as needed

    resourceManager->addTexture(new DEngine::Texture("./working/frame0.png"));
    resourceManager->addTexture(new DEngine::Texture("./working/wall.jpg"));

    DEngine::Shader *quadShader = new DEngine::Shader("./working/quad.vs", "./working/quad.fs");
    DEngine::Shader *lineShader = new DEngine::Shader("./working/line.vs", "./working/line.fs");

    DEngine::QuadRenderer *quadRenderer = new DEngine::QuadRenderer(quadShader, 1024);
    DEngine::LineRenderer *lineRenderer = new DEngine::LineRenderer(lineShader, 1024);

    glm::vec2 windowSize = editor.window.getSize();
    DEngine::Camera2D *camera = new DEngine::Camera2D({0, 0}, 4.0f, windowSize);
    editor.setRenderLogic(
        [&]()
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            DEngine::Renderer::drawCalls = 0;
            quadRenderer->begin(camera);
            quadRenderer->drawQuad(glm::vec3(0, 0, 0) + shadowOffset, resourceManager->getTexture(0), shadowColor);
            quadRenderer->drawQuad(glm::vec3(1, 0, 0) + shadowOffset, resourceManager->getTexture(1), shadowColor);
            quadRenderer->drawQuad(glm::vec3(0, 0, 0), resourceManager->getTexture(0));
            quadRenderer->drawQuad(glm::vec3(1, 0, 0), resourceManager->getTexture(1));
            quadRenderer->end();

            lineRenderer->begin(camera);
            for (int j = -10; j < 10; j++)
            {
                for (int i = -10; i < 10; i++)
                {
                    lineRenderer->drawSquare(glm::vec3(i, j, 0), 1);
                    lineRenderer->drawCircle(glm::vec3(i, j, 0), 0.5, 64);
                }
            }
            lineRenderer->end();

            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            char drawCallsStr[32];
            sprintf(drawCallsStr, "%d", DEngine::Renderer::drawCalls);
            ImGui::Text(drawCallsStr);
            ImGui::End();
        });
    editor.mainLoop();

    return 0;
}
