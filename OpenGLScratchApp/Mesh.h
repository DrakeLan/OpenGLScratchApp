#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVer, unsigned int numberOfIndices, bool tangentFlag = false);
	void RenderMesh();
	void RenderMeshasPatch();
	void RenderMeshInstancing(GLsizei instancingCount);
	void ClearMesh();
	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

