#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    ~Shader();

private:
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif