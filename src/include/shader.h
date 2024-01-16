#pragma once

#include "./utils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

#include <GL/glew.h>

std::string validatePath(const std::string &path)
{
    if (path.empty())
    {
        std::cerr << "Error: Empty path \"" << path << "\"." << std::endl;
        return "";
    }
    if (!std::filesystem::exists(path))
    {
        std::cerr << "Error: Path \"" << path << "\" does not exist." << std::endl;
        return "";
    }
    if (!std::filesystem::is_regular_file(path))
    {
        std::cerr << "Error: Path \"" << path << "\" is not a regular file." << std::endl;
        return "";
    }

    return path;
}

class Shader
{
public:
    Shader(const GLchar*, const GLchar*);
    ~Shader();

    void use() const;
    GLuint program;

    void setBool(const std::string &, bool) const;
    void setInt(const std::string &, GLint) const;
    void setFloat(const std::string &, GLfloat) const;
    void setVec2(const std::string &, const glm::vec2 &) const;
    void setVec2(const std::string &, GLfloat, GLfloat) const;
    void setVec3(const std::string &, const glm::vec3 &) const;
    void setVec3(const std::string &, GLfloat, GLfloat, GLfloat) const;
    void setVec4(const std::string &, const glm::vec4 &) const;
    void setVec4(const std::string &, GLfloat, GLfloat, GLfloat, GLfloat) const;
    void setMat2(const std::string &, const glm::mat2 &) const;
    void setMat3(const std::string &, const glm::mat3 &) const;
    void setMat4(const std::string &, const glm::mat4 &) const;

private:
    void compile(const GLchar*, const GLchar*) const;
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) : program(glCreateProgram())
{
    std::string vertexShaderPath = validatePath(std::string(vertexPath));
    std::string fragmentShaderPath = validatePath(std::string(fragmentPath));

    if (vertexShaderPath.empty() || fragmentShaderPath.empty())
    {
        std::cerr << "Error: Failed to load shader." << std::endl;
        return;
    }

    std::ifstream vertexShaderFile(vertexShaderPath);
    std::ifstream fragmentShaderFile(fragmentShaderPath);

    std::stringstream vertexShaderStream, fragmentShaderStream;
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    compile(vertexShaderStream.str().c_str(), fragmentShaderStream.str().c_str());
}

void Shader::compile(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) const
{
    GLuint vertex, fragment;
    GLint success;
    std::array<GLchar, 512> infoLog = {};

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex, infoLog.size(), nullptr, infoLog.data());
        std::cerr << "Error: Failed to compile vertex shader." << std::endl;
        std::cerr << infoLog.data() << std::endl;
        return;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment, infoLog.size(), nullptr, infoLog.data());
        std::cerr << "Error: Failed to compile fragment shader." << std::endl;
        std::cerr << infoLog.data() << std::endl;
        return;
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, infoLog.size(), nullptr, infoLog.data());
        std::cerr << "Error: Failed to link shader program." << std::endl;
        std::cerr << infoLog.data() << std::endl;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

void Shader::use() const
{
    glUseProgram(program);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), (GLint) value);
}

void Shader::setInt(const std::string &name, GLint value) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, GLfloat value) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, GLfloat x, GLfloat y) const
{
    glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z) const
{
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
