/*
 * Shader.cpp
 *
 *  Created on: 05/03/2018
 *      Author: imasen
 */

#include "shader.h"

#include <iostream>
#include <fstream>
#include <transformation.h>


static void check_shader_error(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string load_shader(const std::string& fileName);
static GLuint create_shader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName) {
	m_program = glCreateProgram();
	m_shaders[0] = create_shader(load_shader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = create_shader(load_shader(fileName +  ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned i = 0; i< NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

        glBindAttribLocation(m_program, 0, "position");
        glBindAttribLocation(m_program, 1, "texCoord");
        glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	check_shader_error(m_program, GL_LINK_STATUS, true, "Error:  Program linking failed: ");

	glValidateProgram(m_program);
	check_shader_error(m_program, GL_VALIDATE_STATUS, true, "Error:  Program is invalid: ");

        m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

Shader::~Shader() {
	glDeleteProgram(m_program);

	for (unsigned i = 0; i< NUM_SHADERS; i++)
	{
            glDetachShader(m_program, m_shaders[i]);
	    glDeleteShader(m_shaders[i]);
	}

}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::update(const Transformation &transform, const Camera &camera)
{
    glm::mat4 model = camera.get_view_projection() * transform.get_model();
    glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

static GLuint create_shader(const std::string& text, GLenum shaderType)
{

	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed!" << std::endl;

	const GLchar *shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);
	check_shader_error(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed!: ");

        return shader;
}

static std::string load_shader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

static void check_shader_error(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}

}
