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
    glm::vec3 shadowOffset = glm::vec3(0.1f, -0.1f, 0); // adjust as needed
    glm::vec4 shadowColor = glm::vec4(0, 0, 0, 0.5f);   // adjust as needed

    resourceManager->addTexture(new DEngine::Texture("./working/frame0.png"));
    resourceManager->addTexture(new DEngine::Texture("./working/wall.jpg"));
    resourceManager->addTexture(new DEngine::Texture("./working/white.png"));

    DEngine::Shader *quadShader = new DEngine::Shader("./working/quad.vs", "./working/quad.fs");
    DEngine::Shader *lineShader = new DEngine::Shader("./working/line.vs", "./working/line.fs");

    DEngine::QuadRenderer *quadRenderer = new DEngine::QuadRenderer(quadShader, 1024);
    DEngine::LineRenderer *lineRenderer = new DEngine::LineRenderer(lineShader, 1024);

    glm::vec2 windowSize = editor.window.getSize();
    DEngine::Camera2D *camera = new DEngine::Camera2D({0, 0}, 4.0f, windowSize);
    std::vector<DEngine::Light> lights;
    for (int i = 0; i < 2; i++)
    {
        lights.push_back({glm::vec3(i * 20 - 10, 0, 0), glm::vec3(1, 1, 1), 0.5f});
    }
    editor.setRenderLogic(
        [&]()
        {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            DEngine::Renderer::drawCalls = 0;
            quadRenderer->addLights(lights);
            quadRenderer->begin(camera);

            // quadRenderer->drawQuad(glm::vec3(0, 0, 0) + shadowOffset, glm::vec3(1, 1, 1), resourceManager->getTexture(0), shadowColor);
            // quadRenderer->drawQuad(glm::vec3(1, 0, 0) + shadowOffset, glm::vec3(1, 1, 1), resourceManager->getTexture(1), shadowColor);
            quadRenderer->drawQuad(glm::vec3(0, 0, 0), glm::vec3(20, 20, 20), resourceManager->getTexture(2), glm::vec4(1, 1, 1, 1));
            quadRenderer->drawQuad(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), resourceManager->getTexture(0));
            quadRenderer->drawQuad(glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), resourceManager->getTexture(1));
            quadRenderer->end();

            lineRenderer->begin(camera);
            for (int i = 0; i < lights.size(); i++)
            {
                lineRenderer->drawCircle(lights[i].position, 0.1);
            }
            lineRenderer->end();

            ImGui::Begin("Hello, world!");

            for (int i = 0; i < lights.size(); i++)
            {
                ImGui::SliderFloat3(("Light " + std::to_string(i)).c_str(), &lights[i].position[0], -50.0f, 50.0f);
            }

            ImGui::End();
        });
    editor.mainLoop();

    return 0;
}
