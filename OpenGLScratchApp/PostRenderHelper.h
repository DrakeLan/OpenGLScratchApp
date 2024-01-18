#pragma once

#include "Mesh.h"
#include "Model.h"

class PostRenderHelper
{	
	public:
		PostRenderHelper();

		void Init();

		Mesh* GetFullquad();
		Model* GetMapCube();

		~PostRenderHelper();

	protected:
		Mesh *fullQuad;
		Model *mapCube;

};

