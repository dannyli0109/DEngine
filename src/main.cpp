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
    DEngine::Shader *lineShader = new DEngine::Shader("./working/line.vs", "./working/line.fs");

    DEngine::QuadRenderer *quadRenderer = new DEngine::QuadRenderer(quadShader, 1024);
    DEngine::LineRenderer *lineRenderer = new DEngine::LineRenderer(lineShader, 1024);

    glm::vec2 windowSize = editor.window.getSize();
    DEngine::Camera2D *camera = new DEngine::Camera2D({0, 0}, 4.0f, windowSize);
    std::vector<DEngine::Light> lights;
    for (int i = 0; i < 5; i++)
    {
        lights.push_back({glm::vec3(i - 5, 0, 0), glm::vec3(1, 1, 1), 1});
    }
    editor.setRenderLogic(
        [&]()
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            quadRenderer->addLights(lights);
            quadRenderer->begin(camera);
            quadRenderer->drawQuad(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), resourceManager->getTexture(0));
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
                ImGui::SliderFloat3(("Light " + std::to_string(i)).c_str(), &lights[i].position[0], -5.0f, 5.0f, 0.1);
            }

            ImGui::End();
        });
    editor.mainLoop();

    return 0;
}
