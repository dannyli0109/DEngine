#include "shader.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include <vector>
#include <sstream>
#include "resourceManager.h"
#include "camera.h"
#include <iostream>
class Texture;

namespace DEngine
{
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;
        float textureIndex;
    };
    class Renderer
    {
    public:
        virtual void begin(Camera *camera) = 0;
        virtual void flush() = 0;
        virtual void beginBatch() = 0;
        virtual void nextBatch() = 0;
        virtual void end() = 0;

    protected:
        Shader *shader;

        VertexBuffer *vertexBuffer;
        IndexBuffer *indexBuffer;
        VertexArray *vertexArrayObject;
    };

    class QuadRenderer : public Renderer
    {
    public:
        QuadRenderer(Shader *shader, int batchSize)
        {
            this->shader = shader;
            maxVertices = batchSize * 4;
            maxIndices = batchSize * 6;

            vertices.resize(maxVertices);
            indices.resize(maxIndices);
            textureSlots.resize(maxTextureSlots);

            textureSlotIndex = 0;

            int offset = 0;
            for (int i = 0; i < maxIndices; i += 6)
            {
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;
                indices[i + 3] = offset + 1;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset + 2;
                offset += 4;
            }

            for (int i = 0; i < maxTextureSlots; i++)
            {
                std::stringstream tex;
                tex << "u_Textures[" << i << "]";
                shader->setUniform(tex.str(), i);
            }

            vertexBuffer = new VertexBuffer(maxVertices, sizeof(QuadVertex));
            indexBuffer = new IndexBuffer(indices.data(), maxIndices);
            vertexArrayObject = new VertexArray();

            vertexArrayObject->bind();
            vertexBuffer->bind();
            indexBuffer->bind();

            vertexBuffer->setAttributePointers(
                {{3, GL_FLOAT, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, position)},
                 {4, GL_FLOAT, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, color)},
                 {2, GL_FLOAT, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, uv)},
                 {1, GL_FLOAT, sizeof(QuadVertex), (const void *)offsetof(QuadVertex, textureIndex)}});

            VertexArray::unbind();
            VertexBuffer::unbind();
            IndexBuffer::unbind();
        }
        ~QuadRenderer()
        {
            delete vertexBuffer;
            delete indexBuffer;
            delete vertexArrayObject;
        }
        QuadRenderer(const QuadRenderer &other) = delete;
        QuadRenderer &operator=(const QuadRenderer &other) = delete;
        QuadRenderer(const QuadRenderer &&other) = delete;
        QuadRenderer &operator=(const QuadRenderer &&other) = delete;

    public:
        virtual void begin(Camera *camera) override
        {
            shader->use();
            shader->setUniform("u_ProjectionMatrix", camera->getProjectionMatrix());
            beginBatch();
        }

        virtual void flush() override
        {
            if (indexCount == 0)
                return;
            vertexBuffer->SetData(vertices.data(), sizeof(QuadVertex), vertexCount);
            for (int i = 0; i < textureSlotIndex; i++)
            {
                textureSlots[i]->bind(i);
            }

            vertexArrayObject->bind();
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

            VertexArray::unbind();
            VertexBuffer::unbind();
            IndexBuffer::unbind();
        }
        virtual void beginBatch() override
        {
            indexCount = 0;
            vertexCount = 0;
            vertices.resize(maxVertices);
            textureSlotIndex = 0;
        }
        virtual void nextBatch() override
        {
            flush();
            beginBatch();
        }
        virtual void end() override
        {
            flush();
        }

    public:
        void drawQuad(const glm::mat4 &transform, Texture *texture, const glm::vec4 &tintColor = glm::vec4(1, 1, 1, 1), bool flipped = false)
        {
            if (indexCount >= maxIndices)
            {
                nextBatch();
            }

            int textureIndex = -1;
            for (int i = 0; i < textureSlotIndex; i++)
            {
                if (*textureSlots[i] == *texture)
                {
                    textureIndex = (float)i;
                    break;
                }
            }

            if (textureIndex == -1)
            {
                if (textureSlotIndex >= maxTextureSlots)
                    nextBatch();

                textureIndex = (float)textureSlotIndex;
                textureSlots[textureSlotIndex] = texture;
                textureSlotIndex++;
            }

            for (int i = 0; i < 4; i++)
            {
                vertices[vertexCount].position = transform * quadPositions[i];
                vertices[vertexCount].color = tintColor;
                vertices[vertexCount].uv = quadUvs[i];
                vertices[vertexCount].textureIndex = textureIndex;
                if (flipped)
                {
                    vertices[vertexCount].uv.x = 1 - vertices[vertexCount].uv.x;
                }
                vertexCount++;
            }
            indexCount += 6;
        }
        void drawQuad(const glm::vec3 &position, Texture *texture, const glm::vec4 &tintColor = glm::vec4(1, 1, 1, 1), bool flipped = false)
        {
            glm::mat4 transform = glm::translate(glm::mat4(1), position);
            drawQuad(transform, texture, tintColor, flipped);
        }

        void drawQuad(const glm::mat4 &transform, const glm::vec4 &tintColor)
        {
            drawQuad(transform, ResourceManager::getInstance()->getTexture(0), tintColor);
        }
        void drawQuad(const glm::vec3 &position, const glm::vec4 &tintColor)
        {
            drawQuad(position, ResourceManager::getInstance()->getTexture(0), tintColor);
        }

    private:
        int maxVertices = 0;
        int maxIndices = 0;
        const int maxTextureSlots = 16;

        std::vector<QuadVertex> vertices;
        std::vector<unsigned short> indices;

        int selectedTexture = 0;
        std::vector<Texture *> textureSlots;
        int textureSlotIndex = 0;
        int indexCount = 0;
        int vertexCount = 0;

        glm::vec4 quadPositions[4] = {
            {-0.5f, 0.5f, 0, 1.0f},
            {0.5f, 0.5f, 0, 1.0f},
            {-0.5f, -0.5f, 0, 1.0f},
            {0.5f, -0.5f, 0, 1.0f}};

        glm::vec2 quadUvs[4] = {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}};
    };
}