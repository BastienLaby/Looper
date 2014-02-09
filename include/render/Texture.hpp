#ifndef _HPP_LOOPER_TEXTURE_HPP_
#define _HPP_LOOPER_TEXTURE_HPP_

#include <GL/gl.h>

#include "opencv2/core/core.hpp"

namespace looper
{

class Texture
{
	GLuint m_textureID;

public:
	Texture()
	{
		glGenTextures(1, &m_textureID);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void setSource(cv::Mat frame)
	{
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    unbind();
	}

	void bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

};

}

#endif