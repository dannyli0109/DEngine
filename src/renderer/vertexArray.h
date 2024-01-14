// C++
#include <glad/glad.h>

namespace DEngine
{
    class VertexArray
    {
    public:
        unsigned int ID;

        VertexArray()
        {
            glGenVertexArrays(1, &ID);
        }

        ~VertexArray()
        {
            glDeleteVertexArrays(1, &ID);
        }

        void bind() const
        {
            glBindVertexArray(ID);
        }

        static void unbind()
        {
            glBindVertexArray(0);
        }
    };
}