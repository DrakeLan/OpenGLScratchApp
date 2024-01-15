#pragma once

#include "Textrue.h"
#include "IBLTexture.h"
#include "Shader.h"


class IBLRender
{
	public:
		IBLRender();
		IBLRender(GLfloat textureWidth, GLfloat textureHeight);
		
		void EquirectangularToCube(Textrue EquirectangularTex);
		void RenderIrradianceMap();

		~IBLRender();

	protected:
		IBLTexture *envCubeMap;
		IBLTexture *irradianceMap;
		Shader* etangToCubeShader;
		Shader* convIrradianceShader;
		


};

