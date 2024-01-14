// C++
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "graphicsInclude.h"

namespace DEngine
{
    class Shader
    {
    public:
        GLuint ID;

        Shader(const char *vertexPath, const char *fragmentPath)
        {
            // 1. Retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure &e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            const char *vShaderCode = vertexCode.c_str();
            const char *fShaderCode = fragmentCode.c_str();

            // 2. compile shaders
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            // vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            // Shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");

            // Delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use()
        {
            glUseProgram(ID);
        }

        GLuint getUniformLocation(std::string varName)
        {
            return glGetUniformLocation(ID, varName.c_str());
        }

        void setUniform(std::string varName, float value)
        {
            GLuint varLoc = glGetUniformLocation(ID, varName.c_str());
            use();
            glUniform1f(varLoc, value);
        }

        void setUniform(std::string varName, int value)
        {
            GLuint varLoc = glGetUniformLocation(ID, varName.c_str());
            use();
            glUniform1i(varLoc, value);
        }

        void setUniform(std::string varName, glm::vec3 value)
        {
            GLuint varLoc = glGetUniformLocation(ID, varName.c_str());
            use();
            glUniform3f(varLoc, value.x, value.y, value.z);
        }

        void setUniform(std::string varName, glm::mat4 value)
        {
            GLuint varLoc = glGetUniformLocation(ID, varName.c_str());
            use();
            glUniformMatrix4fv(varLoc, 1, GL_FALSE, &value[0][0]);
        }

    private:
        void checkCompileErrors(GLuint shader, std::string type)
        {
            GLint success;
            GLchar infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                              << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                              << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
}