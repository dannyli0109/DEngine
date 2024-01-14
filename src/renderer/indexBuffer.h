// C++
#include <glad/glad.h>

namespace DEngine
{
    class IndexBuffer
    {
    public:
        unsigned int ID;
        unsigned int count;

        IndexBuffer(const unsigned short *indices, unsigned int count) : count(count)
        {
            glGenBuffers(1, &ID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        }

        ~IndexBuffer()
        {
            glDeleteBuffers(1, &ID);
        }

        void bind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        }

        static void unbind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        unsigned int getCount() const
        {
            return count;
        }
    };
}