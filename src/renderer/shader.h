#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "graphics_include.h"

namespace DEngine
{
    typedef struct Shader
    {
        unsigned int id;
        const char *vertexPath;
        const char *fragmentPath;
    } Shader;

    void checkCompileErrors(unsigned int shader, std::string type);

    Shader *createShader(const char *vertexPath, const char *fragmentPath)
    {
        Shader *shader = new Shader();
        shader->vertexPath = vertexPath;
        shader->fragmentPath = fragmentPath;

        // 1. retrieve the vertex/fragment source code from filePath
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
            return NULL;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // shader Program
        shader->id = glCreateProgram();
        glAttachShader(shader->id, vertex);
        glAttachShader(shader->id, fragment);
        glLinkProgram(shader->id);
        checkCompileErrors(shader->id, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
                          << "\n"
                          << infoLog
                          << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
                          << "\n"
                          << infoLog
                          << "\n -- --------------------------------------------------- -- "
                          << std::endl;
            }
        }
    }

    void useShader(Shader *shader)
    {
        glUseProgram(shader->id);
    }

    void setInt(Shader *shader, std::string name, int value)
    {
        glUniform1i(glGetUniformLocation(shader->id, name.c_str()), value);
    }

    void setMat4(Shader *shader, std::string name, glm::mat4 value)
    {
        GLuint varloc = glGetUniformLocation(shader->id, name.c_str());
        useShader(shader);
        glUniformMatrix4fv(varloc, 1, GL_FALSE, &value[0][0]);
    }

    void setVec3(Shader *shader, std::string name, glm::vec3 value)
    {
        GLuint varloc = glGetUniformLocation(shader->id, name.c_str());
        useShader(shader);
        glUniform3fv(varloc, 1, &value[0]);
    }

    void setVec4(Shader *shader, std::string name, glm::vec4 value)
    {
        GLuint varloc = glGetUniformLocation(shader->id, name.c_str());
        useShader(shader);
        glUniform4fv(varloc, 1, &value[0]);
    }

    void setFloat(Shader *shader, std::string name, float value)
    {
        GLuint varloc = glGetUniformLocation(shader->id, name.c_str());
        useShader(shader);
        glUniform1f(varloc, value);
    }

    void destroyShader(Shader *shader)
    {
        glDeleteProgram(shader->id);
        free(shader);
    }
}