#ifndef _HPP_LOOPER_RENDERER_HPP_
#define _HPP_LOOPER_RENDERER_HPP_

#include <GL/gl.h>
#include "Shader.hpp"

namespace looper
{

class Renderer
{

public:
	
	void beginDraw()
	{
		glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void endDraw()
	{
		GLenum err = glGetError();
        if(err != GL_NO_ERROR)
        {
        	fprintf(stderr, "OpenGL Error : %s\n", gluErrorString(err));
        }
	}

	void useShaderProgram(Shader shader)
	{
		glUseProgram(shader.getProgram());
	}

};

}

#endif