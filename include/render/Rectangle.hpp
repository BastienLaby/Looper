#ifndef _HPP_LOOPER_RECTANGLE_HPP_
#define _HPP_LOOPER_RECTANGLE_HPP_

#include <GL/gl.h>

namespace looper
{

class Rectangle
{
	GLuint m_vbos[4];
	GLuint m_vao;

	void sendData(glm::vec3 topleft, glm::vec3 topright, glm::vec3 botright, glm::vec3 botleft)
	{
		int plane_triangleList[] = {0, 1, 2, 0, 2, 3}; 
	    float plane_uvs[] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f};
	    float plane_vertices[] =
	    {	topleft.x, topleft.y, topleft.z,
			topright.x, topright.y, topright.z,
			botright.x, botright.y, botright.z,
			botleft.x, botleft.y, botleft.z
	    };
	    float plane_normals[] = {0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0};

		glBindVertexArray(m_vao);

	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[0]);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_triangleList), plane_triangleList, GL_STATIC_DRAW);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, (void*)0);

	    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[3]);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_uvs), plane_uvs, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*2, (void*)0);

	    glBindVertexArray(0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

public:
	Rectangle()
	{
		glGenBuffers(4, m_vbos);
		glGenVertexArrays(1, &m_vao);
		sendData(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0));
	}

	void setCorners(glm::vec3 topleft, glm::vec3 topright, glm::vec3 botright, glm::vec3 botleft)
	{
		sendData(topleft, topright, botright, botleft);
	}

	void setCorners(cv::Point2f topleft, cv::Point2f topright, cv::Point2f botright, cv::Point2f botleft)
	{
		glm::vec3 tl(topleft.x, topleft.y, 0);
		glm::vec3 tr(topright.x, topright.y, 0);
		glm::vec3 br(botright.x, botright.y, 0);
		glm::vec3 bl(botleft.x, botleft.y, 0);
		sendData(tl, tr, br, bl);
	}

	void resetCorners()
	{
		sendData(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0));
	}

	void draw()
	{
		glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

}

#endif