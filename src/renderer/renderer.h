#pragma once

#include "graphics_include.h"
#include <vector>
#include "shader.h"
#include <sstream>
#include <iostream>
#include "camera.h"
#include "texture.h"

namespace DEngine
{
    typedef struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;
        float textureIndex;
    } QuadVertex;

    typedef struct Light
    {
        glm::vec3 position;
        glm::vec4 color;
        float intensity;
    } Light;

    typedef struct QuadRenderer
    {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader *shader;
        glm::mat4 transform = glm::mat4(1.0f);
        int maxVertices;
        int maxIndices;
        std::vector<QuadVertex> vertices;
        std::vector<unsigned int> indices;
        int maxVertices = 0;
        int maxIndices = 0;
        const int maxTextureSlots = 16;

        std::vector<QuadVertex> vertices;
        std::vector<unsigned short> indices;
        std::vector<Light> lights;

        int selectedTexture = 0;
        std::vector<Texture *> textureSlots;
        int textureSlotIndex = 0;
        int indexCount = 0;
        int vertexCount = 0;

        glm::vec2 quadUvs[4] = {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}};
    } QuadRenderer;

    QuadRenderer *createQuadRenderer(Shader *shader, int batchSize)
    {
        QuadRenderer *renderer = (QuadRenderer *)malloc(sizeof(QuadRenderer));
        renderer->maxVertices = batchSize * 4;
        renderer->maxIndices = batchSize * 6;
        renderer->vertices.resize(renderer->maxVertices);
        renderer->indices.resize(renderer->maxIndices);
        renderer->textureSlots.resize(renderer->maxTextureSlots);
        renderer->textureSlotIndex = 0;
        int offset = 0;
        for (int i = 0; i < renderer->maxIndices; i += 6)
        {
            renderer->indices[i + 0] = offset + 0;
            renderer->indices[i + 1] = offset + 1;
            renderer->indices[i + 2] = offset + 2;
            renderer->indices[i + 3] = offset + 2;
            renderer->indices[i + 4] = offset + 1;
            renderer->indices[i + 5] = offset + 3;
            offset += 4;
        }

        for (int i = 0; i < renderer->maxTextureSlots; i++)
        {
            std::stringstream tex;
            tex << "u_Textures[" << i << "]";
            shader->setUniform(tex.str(), i);
        }

        // Create VAO
        glGenVertexArrays(1, &renderer->VAO);
        glBindVertexArray(renderer->VAO);

        // Create VBO
        glGenBuffers(1, &renderer->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
        glBufferData(GL_ARRAY_BUFFER, renderer->maxVertices * sizeof(QuadVertex), NULL, GL_DYNAMIC_DRAW);

        // Create EBO
        glGenBuffers(1, &renderer->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderer->maxIndices * sizeof(unsigned int), NULL, GL_STATIC_DRAW);

        // Set vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, color));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, uv));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, textureIndex));
        glEnableVertexAttribArray(3);

        // Unbind VAO
        glBindVertexArray(0);

        // Set shader program
        renderer->shader = shader;

        return renderer;
    }

    void destroyQuadRenderer(QuadRenderer *renderer)
    {
        glDeleteVertexArrays(1, &renderer->VAO);
        glDeleteBuffers(1, &renderer->VBO);
        glDeleteBuffers(1, &renderer->EBO);
        free(renderer);
    }

    void beginBatch(QuadRenderer *renderer)
    {
        renderer->indexCount = 0;
        renderer->vertexCount = 0;
        renderer->textureSlotIndex = 0;
        renderer->vertices.resize(renderer->maxVertices);
    }

    void beginQuadRenderer(QuadRenderer *renderer, Camera *camera)
    {
        useShader(renderer->shader);
        setMat4(renderer->shader, "u_ProjectionMatrix", getProjectionMatrix(camera));
        for (int i = 0; i < renderer->lights.size(); i++)
        {
            std::stringstream light;
            light << "u_Lights[" << i << "]";
            setVec3(renderer->shader, light.str() + ".position", renderer->lights[i].position);
            setVec4(renderer->shader, light.str() + ".color", renderer->lights[i].color);
            setFloat(renderer->shader, light.str() + ".intensity", renderer->lights[i].intensity);
        }

        glBindVertexArray(renderer->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);

        beginBatch(renderer);
    }

    void addLights(QuadRenderer *renderer, std::vector<Light> &lights)
    {
        renderer->lights = lights;
    }

    void flush(QuadRenderer *renderer)
    {
        if (renderer->indexCount == 0)
            return;

        for (int i = 0; i < renderer->textureSlotIndex; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, renderer->textureSlots[i]->id);
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->vertexCount * sizeof(QuadVertex), renderer->vertices.data());
        glBindVertexArray(renderer->VAO);
        glDrawElements(GL_TRIANGLES, renderer->indexCount, GL_UNSIGNED_SHORT, NULL);

        for (int i = 0; i < renderer->textureSlotIndex; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindVertexArray(0);
    }

    void endQuadRenderer(QuadRenderer *renderer)
    {
        flush(renderer);
    }

    void nextBatch(QuadRenderer *renderer)
    {
        endQuadRenderer(renderer);
        beginBatch(renderer);
    }
}