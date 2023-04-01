#pragma once

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <array>

class Shader
{
public:
	Shader();
	~Shader();

	void use() const;
	GLuint program;

	void setBool(const std::string&, bool) const;
	void setInt(const std::string&, int) const;
	void setFloat(const std::string&, float) const;

private:
	const GLchar* vertexShaderSource = R"glsl(
		#version 330 core

		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoords;

		out vec2 TexCoords;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0f);
			TexCoords = texCoords;
		}
	)glsl";

	const GLchar* fragmentShaderSource = R"glsl(
		#version 330 core

		in vec2 TexCoords;
		out vec4 color;

		uniform sampler2D texture_diffuse;

		void main()
		{
			color = vec4(texture(texture_diffuse, TexCoords));
		}
	)glsl";
};

Shader::Shader()
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
		infoLog[infoLog.size() - 1] = '\n';

		std::cerr << "Error: Vertex shader compilation failed.\n"
				  << infoLog.data() << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, infoLog.size(), nullptr, infoLog.data());
		infoLog[infoLog.size() - 1] = '\n';

		std::cerr << "Error: Fragment shader compilation failed.\n"
				  << infoLog.data() << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, infoLog.size(), nullptr, infoLog.data());
		infoLog[infoLog.size() - 1] = '\n';

		std::cerr << "Error: Shader program linking failed.\n" << infoLog.data()
				  << std::endl;
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

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
	
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
	
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
	
}
