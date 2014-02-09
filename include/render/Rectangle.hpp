#ifndef _HPP_LOOPER_RECTANGLE_HPP_
#define _HPP_LOOPER_RECTANGLE_HPP_

#include <GL/gl.h>

namespace looper
{

class Rectangle
{
	GLuint m_vbos[4];
	GLuint m_vao;
public:
	Rectangle()
	{

	    int plane_triangleList[] = {0, 1, 2, 0, 2, 3}; 
	    float plane_uvs[] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f};
	    float plane_vertices[] = {-1, 1, 0.0, 1, 1, 0.0, 1, -1, 0.0, -1, -1, 0.0};
	    float plane_normals[] = {0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0};
		
		glGenBuffers(4, m_vbos);
		glGenVertexArrays(1, &m_vao);

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

	void draw()
	{
		glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

}

#endif