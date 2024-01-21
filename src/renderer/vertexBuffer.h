// C++
#include <glad/glad.h>
#include <vector>
#include "vertexAttribute.h"

namespace DEngine
{
    class VertexBuffer
    {
    public:
        unsigned int ID;
        bool initialised = false;

        // Dynamic draw
        VertexBuffer(unsigned int vertexCount, GLsizeiptr size)
        {
            if (initialised)
            {
                glDeleteBuffers(1, &ID);
            }
            initialised = true;

            glGenBuffers(1, &ID);
            bind();
            glBufferData(GL_ARRAY_BUFFER, vertexCount * size, nullptr, GL_DYNAMIC_DRAW);
            unbind();
        }

        // Static draw
        VertexBuffer(unsigned int vertexCount, GLsizeiptr size, const void *data)
        {
            if (initialised)
            {
                glDeleteBuffers(1, &ID);
            }
            initialised = true;

            glGenBuffers(1, &ID);
            bind();
            glBufferData(GL_ARRAY_BUFFER, vertexCount * size, data, GL_STATIC_DRAW);
            unbind();
        }

        void init(unsigned int vertexCount, GLsizeiptr size)
        {
            bind();
            glBufferData(GL_ARRAY_BUFFER, vertexCount * size, nullptr, GL_DYNAMIC_DRAW);
            unbind();
        }

        ~VertexBuffer()
        {
            if (initialised)
            {
                glDeleteBuffers(1, &ID);
            }
        }

        void setAttributePointers(std::vector<VertexAttribute> attributes)
        {
            for (int i = 0; i < attributes.size(); i++)
            {
                VertexAttribute attribute = attributes[i];
                glVertexAttribPointer(i, attribute.size, attribute.type, GL_FALSE, attribute.stride, attribute.pointer);
                glEnableVertexAttribArray(i);
            }
        }

        void bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, ID);
        }

        static void unbind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void SetData(const void *data, GLsizeiptr size, int vertexCount)
        {
            bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * size, data);
            unbind();
        }
    };
}