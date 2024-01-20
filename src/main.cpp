#include "editor/editor.h"
#include "renderer/renderer.h"
#include "renderer/resourceManager.h"
#include "renderer/camera.h"
#include "renderer/frameBuffer.h"
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
    DEngine::Shader *shadowShader = new DEngine::Shader("./working/shadow.vs", "./working/shadow.fs");

    DEngine::QuadRenderer *quadRenderer = new DEngine::QuadRenderer(quadShader, 1024);
    DEngine::QuadRenderer *quadRenderer2 = new DEngine::QuadRenderer(quadShader, 1024);

    DEngine::LineRenderer *lineRenderer = new DEngine::LineRenderer(lineShader, 1024);
    DEngine::QuadRenderer *shadowRenderer = new DEngine::QuadRenderer(shadowShader, 1024);

    glm::vec2 windowSize = editor.window.getSize();
    DEngine::Camera2D *camera = new DEngine::Camera2D({0, 0}, 4.0f, windowSize);

    DEngine::Framebuffer *framebuffer = new DEngine::Framebuffer(windowSize.x, windowSize.y);
    std::vector<DEngine::Light> lights;
    glm::vec4 quadPositions[4] = {
        {-0.5f, 0.5f, 0, 1.0f},
        {0.5f, 0.5f, 0, 1.0f},
        {-0.5f, -0.5f, 0, 1.0f},
        {0.5f, -0.5f, 0, 1.0f}};
    for (int i = 0; i < 2; i++)
    {
        lights.push_back({glm::vec3(i * 10 - 5, 0, 0), glm::vec3(1, 1, 1), 0.5f});
    }
    editor.setRenderLogic(
        [&]()
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glViewport(0, 0, windowSize.x, windowSize.y);
            framebuffer->bind();
            ImGui::Begin("scene");
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            DEngine::Renderer::drawCalls = 0;

            quadRenderer->addLights(lights);
            quadRenderer->begin(camera);
            quadRenderer->drawQuad(glm::vec3(0, 0, 0), glm::vec3(20, 20, 20), resourceManager->getTexture(2), glm::vec4(1, 1, 1, 1));
            // quadRenderer->drawQuad(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), resourceManager->getTexture(0));
            quadRenderer->end();

            shadowRenderer->begin(camera);
            glm::vec4 pos = glm::vec4(0, 0, 0, 0);
            // get four corners of quad
            glm::vec4 topLeft = pos + glm::vec4(-0.5f, 0.5f, 0, 0);
            glm::vec4 topRight = pos + glm::vec4(0.5f, 0.5f, 0, 0);
            glm::vec4 bottomLeft = pos + glm::vec4(-0.5f, -0.5f, 0, 0);
            glm::vec4 bottomRight = pos + glm::vec4(0.5f, -0.5f, 0, 0);

            glm::vec4 sides[4] = {
                topLeft,
                topRight,
                bottomRight,
                bottomLeft};
            static float shadowOpacity = 1.0f; // Initial value

            for (int i = 0; i < 4; i++)
            {
                DEngine::Light light = lights[0];
                glm::vec4 quadVertex[4] = {};
                glm::vec4 side = sides[i];
                glm::vec4 nextSide = sides[(i + 1) % 4];
                // setup quad vertex
                quadVertex[0] = side;
                quadVertex[1] = side + (side - glm::vec4(light.position, 0)) * 100.0f; // extend to the direction of light
                quadVertex[2] = nextSide;                                              // extend to the direction of light
                quadVertex[3] = nextSide + (nextSide - glm::vec4(light.position, 0)) * 100.0f;
                shadowRenderer->drawQuad(quadVertex[0], quadVertex[1], quadVertex[2], quadVertex[3], glm::vec4(0, 0, 0, shadowOpacity));
            }
            shadowRenderer->end();

            quadRenderer2->begin(camera);
            quadRenderer2->addLights(lights);
            quadRenderer2->drawQuad(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), resourceManager->getTexture(0));
            quadRenderer2->end();

            lineRenderer->begin(camera);
            for (int i = 0; i < lights.size(); i++)
            {
                lineRenderer->drawCircle(lights[i].position, 0.1);
            }
            lineRenderer->end();

            DEngine::Framebuffer::unbind();
            ImGui::Image((ImTextureID)framebuffer->getTextureID(), {windowSize.x, windowSize.y}, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();

            ImGui::Begin("Hello, world!");

            for (int i = 0; i < lights.size(); i++)
            {
                ImGui::SliderFloat2(("Light " + std::to_string(i)).c_str(), &lights[i].position[0], -50.0f, 50.0f);
            }
            // ImGui::SliderFloat("Shadow Opacity", &shadowOpacity, 0.0f, 1.0f);

            ImGui::End();
        });
    editor.mainLoop();

    return 0;
}
