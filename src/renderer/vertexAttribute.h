#include "graphicsInclude.h"

namespace DEngine
{
    struct VertexAttribute
    {
        GLint size;
        GLenum type;
        GLsizei stride;
        const void *pointer;
    };
}