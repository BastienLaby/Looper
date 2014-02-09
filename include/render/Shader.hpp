#ifndef _HPP_LOOPER_SHADER_HPP_
#define _HPP_LOOPER_SHADER_HPP_

#include <GL/gl.h>
#include "stb/stb_image.h"
#include "ShaderTools.hpp"

namespace looper
{

class Shader
{

	ShaderGLSL m_shader;
	std::map<const char*, GLuint> m_locations;

public:

	void load(const char* shaderFile)
	{
	    int status = load_shader_from_file(m_shader, shaderFile, ShaderGLSL::VERTEX_SHADER | ShaderGLSL::FRAGMENT_SHADER | ShaderGLSL::GEOMETRY_SHADER);
	    if (status == -1)
	    {
	        fprintf(stderr, "Error on loading  %s\n", shaderFile);
	        exit( EXIT_FAILURE );
	    }
	}

	GLuint getProgram()
	{
		return m_shader.program;
	}

	void createShaderLocation(const char* uniformName)
	{
		GLuint location = glGetUniformLocation(m_shader.program, uniformName);
		m_locations.insert(std::pair<const char*, GLuint>(uniformName, location));
	}

	void sendUniformMatrix4fv(const char* uniformName, glm::mat4 mat)
	{
		glUniformMatrix4fv(m_locations.at(uniformName), 1, 0, glm::value_ptr(mat));
	}

	void sendUniformInteger(const char* uniformName, int i)
	{
		glUniform1i(m_locations.at(uniformName), i);
	}
};

}

#endif