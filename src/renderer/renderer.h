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
        std::vector<QuadVertex> vertices;
        std::vector<unsigned int> indices;
        int maxVertices = 0;
        int maxIndices = 0;
        int maxTextureSlots = 16;

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

    Light *createLight(glm::vec3 position, glm::vec4 color, float intensity)
    {
        Light *light = (Light *)malloc(sizeof(Light));
        light->position = position;
        light->color = color;
        light->intensity = intensity;
        return light;
    }

    QuadRenderer *createQuadRenderer(Shader *shader, int batchSize)
    {
        QuadRenderer *renderer = new QuadRenderer();
        renderer->maxVertices = batchSize * 4;
        renderer->maxIndices = batchSize * 6;
        renderer->maxTextureSlots = 16;
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
            renderer->indices[i + 3] = offset + 1;
            renderer->indices[i + 4] = offset + 3;
            renderer->indices[i + 5] = offset + 2;
            offset += 4;
        }

        for (int i = 0; i < renderer->maxTextureSlots; i++)
        {
            std::stringstream tex;
            tex << "u_Textures[" << i << "]";
            DEngine::setInt(shader, tex.str(), i);
            std::cout << tex.str() << std::endl;
        }

        // // Create VAO
        // glGenVertexArrays(1, &renderer->VAO);
        // glBindVertexArray(renderer->VAO);

        // // Create VBO
        // glGenBuffers(1, &renderer->VBO);
        // glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
        // glBufferData(GL_ARRAY_BUFFER, renderer->maxVertices * sizeof(QuadVertex), NULL, GL_DYNAMIC_DRAW);

        // // Create EBO
        // glGenBuffers(1, &renderer->EBO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderer->maxIndices * sizeof(unsigned int), NULL, GL_STATIC_DRAW);

        // // Set vertex attributes
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, position));
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, color));
        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, uv));
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void *)offsetof(QuadVertex, textureIndex));
        // glEnableVertexAttribArray(3);

        // // Unbind VAO
        // glBindVertexArray(0);

        // // Unbind VBO
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // // Unbind EBO
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
            setVec3(renderer->shader, (light.str() + ".position"), renderer->lights[i].position);
            setVec4(renderer->shader, (light.str() + ".color"), renderer->lights[i].color);
            setFloat(renderer->shader, (light.str() + ".intensity"), renderer->lights[i].intensity);
        }

        glBindVertexArray(renderer->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
        // Clear VBO
        glBufferData(GL_ARRAY_BUFFER, renderer->maxVertices * sizeof(QuadVertex), NULL, GL_DYNAMIC_DRAW);
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

    void drawQuad(QuadRenderer *renderer, const glm::mat4 &transform, Texture *texture, const glm::vec4 &tintColor = glm::vec4(1, 1, 1, 1), bool flipped = false)
    {
        if (renderer->indexCount >= renderer->maxIndices)
            nextBatch(renderer);

        float textureIndex = 0.0f;
        for (int i = 0; i < renderer->textureSlotIndex; i++)
        {
            if (renderer->textureSlots[i]->id == texture->id)
            {
                textureIndex = (float)(i + 1);
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (renderer->textureSlotIndex >= renderer->maxTextureSlots)
                nextBatch(renderer);

            textureIndex = (float)renderer->textureSlotIndex + 1;
            renderer->textureSlots[renderer->textureSlotIndex] = texture;
            renderer->textureSlotIndex++;
        }

        glm::vec4 color = tintColor;
        glm::vec2 size = {1, 1};
        glm::vec2 position = {0, 0};
        glm::vec2 uv[4] = {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}};

        if (flipped)
        {
            uv[0] = {1, 1};
            uv[1] = {0, 1};
            uv[2] = {1, 0};
            uv[3] = {0, 0};
        }

        renderer->vertices[renderer->vertexCount + 0] = {{transform * glm::vec4(position.x, position.y, 0, 1)}, color, uv[0], textureIndex};
        renderer->vertices[renderer->vertexCount + 1] = {{transform * glm::vec4(position.x, position.y + size.y, 0, 1)}, color, uv[1], textureIndex};
        renderer->vertices[renderer->vertexCount + 2] = {{transform * glm::vec4(position.x + size.x, position.y, 0, 1)}, color, uv[2], textureIndex};
        renderer->vertices[renderer->vertexCount + 3] = {{transform * glm::vec4(position.x + size.x, position.y + size.y, 0, 1)}, color, uv[3], textureIndex};
        renderer->indexCount += 6;
        renderer->vertexCount += 4;
    }

    void drawQuad(QuadRenderer *renderer, const glm::vec3 &position, const glm::vec3 &scale, Texture *texture, const glm::vec4 &tintColor = glm::vec4(1, 1, 1, 1), bool flipped = false)
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
        drawQuad(renderer, transform, texture, tintColor, flipped);
    }
}