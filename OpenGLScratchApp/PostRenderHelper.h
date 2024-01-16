#pragma once

#include "Mesh.h"
#include "Model.h"

class PostRenderHelper
{	
	public:
		PostRenderHelper();

		Mesh* GetFullquad();
		Model* GetMapCube();

		~PostRenderHelper();

	protected:
		Mesh *fullQuad;
		Model* mapCube;

};

