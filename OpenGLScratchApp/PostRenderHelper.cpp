#include "PostRenderHelper.h"


PostRenderHelper::PostRenderHelper() {

	unsigned int QuadIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat QuadVertices[] =
	{
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,

	};


	fullQuad = new Mesh();
	fullQuad->CreateMesh(QuadVertices, QuadIndices, 32, 6);

	mapCube = new Model();
	mapCube->LoadModel("Models/Cube.obj");
}

Mesh* PostRenderHelper::GetFullquad()
{
	if (fullQuad != nullptr)
	{
		return fullQuad;
	}
	else
	{
		return nullptr;
	}

}

Model* PostRenderHelper::GetMapCube()
{
	if (mapCube != nullptr)
	{
		return mapCube;
	}
	else
	{
		return nullptr;
	}
}


PostRenderHelper::~PostRenderHelper()
{
	delete fullQuad;
	fullQuad = nullptr;

	delete mapCube;
	mapCube = nullptr;
}


