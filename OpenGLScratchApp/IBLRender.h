#pragma once

#include "Textrue.h"
#include "IBLTexture.h"
#include "Shader.h"
#include "PostRenderHelper.h"


class IBLRender
{
	public:
		IBLRender();
		IBLRender(GLfloat textureWidth, GLfloat textureHeight);
		
		void EquirectangularToCube(Textrue EquirectangularTex);
		void RenderIrradianceMap();

		~IBLRender();

	private:
		GLuint uniformEquTexture;
		GLuint uniformCubeMap;
		GLuint uniformIrradianceMap;

	protected:
		IBLTexture *envCubeMap;
		IBLTexture *irradianceMap;
		Shader* equToCubeShader;
		Shader* convIrradianceShader;
		PostRenderHelper renderHelper;
		


};

