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

    struct LineVertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    struct Light
    {
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
    };

    class Renderer
    {
    public:
        virtual void begin(Camera *camera) = 0;
        virtual void flush() = 0;
        virtual void beginBatch() = 0;
        virtual void nextBatch() = 0;
        virtual void end() = 0;
        static int drawCalls;

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
            for (int i = 0; i < lights.size(); i++)
            {
                std::stringstream lightPos;
                lightPos << "u_Lights[" << i << "].position";
                shader->setUniform(lightPos.str(), lights[i].position);
                std::stringstream lightColor;
                lightColor << "u_Lights[" << i << "].color";
                shader->setUniform(lightColor.str(), lights[i].color);
                std::stringstream lightIntensity;
                lightIntensity << "u_Lights[" << i << "].intensity";
                shader->setUniform(lightIntensity.str(), lights[i].intensity);
            }

            vertexBuffer->init(maxVertices, sizeof(QuadVertex));
            vertexArrayObject->bind();
            vertexBuffer->bind();
            indexBuffer->bind();

            VertexArray::unbind();
            VertexBuffer::unbind();
            IndexBuffer::unbind();

            beginBatch();
        }

        void addLights(const std::vector<Light> &lights)
        {
            this->lights = lights;
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
            Renderer::drawCalls++;
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

            VertexBuffer::unbind();
            IndexBuffer::unbind();
            VertexArray::unbind();
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
        void drawQuad(glm::vec4 quadPositions[], const glm::mat4 &transform, Texture *texture, const glm::vec4 &tintColor = glm::vec4(1, 1, 1, 1), bool flipped = false)
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
        void drawQuad(const glm::vec3 &position, const glm::vec3 &scale, Texture *texture, const glm::vec4 &tintColor = glm::vec4(1, 1, 1, 1), bool flipped = false)
        {
            glm::mat4 transform = glm::scale(glm::mat4(1), scale);
            transform = glm::translate(transform, position);
            glm::vec4 quadPositions[4] = {
                {-0.5f, 0.5f, 0, 1.0f},
                {0.5f, 0.5f, 0, 1.0f},
                {-0.5f, -0.5f, 0, 1.0f},
                {0.5f, -0.5f, 0, 1.0f}};
            drawQuad(quadPositions, transform, texture, tintColor, flipped);
        }

        void drawQuad(const glm::mat4 &transform, const glm::vec4 &tintColor)
        {
            glm::vec4 quadPositions[4] = {
                {-0.5f, 0.5f, 0, 1.0f},
                {0.5f, 0.5f, 0, 1.0f},
                {-0.5f, -0.5f, 0, 1.0f},
                {0.5f, -0.5f, 0, 1.0f}};
            drawQuad(quadPositions, transform, ResourceManager::getInstance()->getTexture(2), tintColor);
        }
        void drawQuad(const glm::vec3 &position, const glm::vec4 &tintColor)
        {
            drawQuad(position, glm::vec3(1, 1, 1), ResourceManager::getInstance()->getTexture(3), tintColor);
        }
        void drawQuad(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec4 &tintColor)
        {
            drawQuad(position, scale, ResourceManager::getInstance()->getTexture(2), tintColor);
        }
        void drawQuad(const glm::vec4 &p1, const glm::vec4 &p2, const glm::vec4 &p3, const glm::vec4 &p4, Texture *texture, const glm::vec4 &tintColor)
        {
            glm::vec4 quadPositions[4] = {
                p1,
                p2,
                p3,
                p4};
            drawQuad(quadPositions, glm::mat4(1), texture, tintColor);
        }

        void drawQuad(const glm::vec4 &p1, const glm::vec4 &p2, const glm::vec4 &p3, const glm::vec4 &p4, const glm::vec4 &tintColor)
        {
            glm::vec4 quadPositions[4] = {
                p1,
                p2,
                p3,
                p4};
            drawQuad(quadPositions, glm::mat4(1), ResourceManager::getInstance()->getTexture(2), tintColor);
        }

    private:
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

        // glm::vec4 quadPositions[4] = {
        //     {-0.5f, 0.5f, 0, 1.0f},
        //     {0.5f, 0.5f, 0, 1.0f},
        //     {-0.5f, -0.5f, 0, 1.0f},
        //     {0.5f, -0.5f, 0, 1.0f}};

        glm::vec2 quadUvs[4] = {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}};
    };

    class LineRenderer : public Renderer
    {
    public:
        LineRenderer(Shader *shader, int batchSize)
        {
            this->shader = shader;
            maxVertices = batchSize * 2;
            maxIndices = batchSize * 2;

            vertices.resize(maxVertices);
            indices.resize(maxIndices);

            int offset = 0;
            for (int i = 0; i < maxIndices; i += 2)
            {
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                offset += 2;
            }

            vertexBuffer = new VertexBuffer(maxVertices, sizeof(LineVertex));
            indexBuffer = new IndexBuffer(indices.data(), maxIndices);
            vertexArrayObject = new VertexArray();

            vertexArrayObject->bind();
            vertexBuffer->bind();
            indexBuffer->bind();

            vertexBuffer->setAttributePointers(
                {{3, GL_FLOAT, sizeof(LineVertex), (const void *)offsetof(LineVertex, position)},
                 {4, GL_FLOAT, sizeof(LineVertex), (const void *)offsetof(LineVertex, color)}});

            VertexArray::unbind();
            VertexBuffer::unbind();
            IndexBuffer::unbind();
        }
        ~LineRenderer()
        {
            delete vertexBuffer;
            delete indexBuffer;
            delete vertexArrayObject;
        }
        LineRenderer(const LineRenderer &other) = delete;
        LineRenderer &operator=(const LineRenderer &other) = delete;
        LineRenderer(const LineRenderer &&other) = delete;
        LineRenderer &operator=(const LineRenderer &&other) = delete;

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
            vertexBuffer->SetData(vertices.data(), sizeof(LineVertex), vertexCount);

            vertexArrayObject->bind();
            Renderer::drawCalls++;
            glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_SHORT, 0);

            VertexArray::unbind();
            VertexBuffer::unbind();
            IndexBuffer::unbind();
        }
        virtual void beginBatch() override
        {
            indexCount = 0;
            vertexCount = 0;
            vertices.resize(maxVertices);
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

        void drawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec4 &color = glm::vec4(1, 1, 1, 1))
        {
            if (indexCount >= maxIndices)
            {
                nextBatch();
            }

            vertices[vertexCount].position = start;
            vertices[vertexCount].color = color;
            vertexCount++;

            vertices[vertexCount].position = end;
            vertices[vertexCount].color = color;
            vertexCount++;

            indexCount += 2;
        }

        void drawCircle(const glm::vec3 &center, float size, int segmentCount = 64, const glm::vec4 &color = glm::vec4(1, 1, 1, 1))
        {
            float cosAngle = cos(2 * 3.14159f / segmentCount);
            float sinAngle = sin(2 * 3.14159f / segmentCount);

            glm::mat3 rotMat = {{cosAngle, -sinAngle, 0}, {sinAngle, cosAngle, 0}, {0, 0, 0}};

            glm::vec3 plotPoint(0, size, 0);

            for (int i = 0; i <= segmentCount; i++)
            {
                glm::vec3 p1 = center + plotPoint;
                plotPoint = rotMat * plotPoint;
                glm::vec3 p2 = center + plotPoint;
                drawLine(p1, p2, color);
            }
        }

        void drawSquare(const glm::vec3 &center, float size, const glm::vec4 &color = glm::vec4(1, 1, 1, 1))
        {
            glm::vec3 p1(center.x - size / 2, center.y - size / 2, center.z);
            glm::vec3 p2(center.x + size / 2, center.y - size / 2, center.z);
            glm::vec3 p3(center.x + size / 2, center.y + size / 2, center.z);
            glm::vec3 p4(center.x - size / 2, center.y + size / 2, center.z);

            drawLine(p1, p2, color);
            drawLine(p2, p3, color);
            drawLine(p3, p4, color);
            drawLine(p4, p1, color);
        }

    private:
        int maxVertices = 0;
        int maxIndices = 0;

        std::vector<LineVertex> vertices;
        std::vector<unsigned short> indices;

        int indexCount = 0;
        int vertexCount = 0;
    };
}
int DEngine::Renderer::drawCalls = 0;