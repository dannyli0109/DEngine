#include "graphics_include.h"
#include <vector>

namespace DEngine
{
    typedef struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;
        float textureIndex;
    } QuadVertex;

    typedef struct QuadRenderer
    {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int shaderProgram;
        glm::mat4 transform = glm::mat4(1.0f);
        int maxVertices;
        int maxIndices;
        const int maxTextureSlots = 16;
        std::vector<QuadVertex> vertices;
        std::vector<unsigned int> indices;

        glm::vec2 quadUvs[4] = {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}};
    } QuadRenderer;

}