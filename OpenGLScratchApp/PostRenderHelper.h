#pragma once

#include "Mesh.h"

class PostRenderHelper
{
	PostRenderHelper();

	Mesh* GetFullquad();

	~PostRenderHelper();

	protected:
		Mesh *fullQuad;


};

