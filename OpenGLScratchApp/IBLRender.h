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
		
		void EquirectangularToCubePass(Textrue EquirectangularTex);
		void RenderIrradianceMapPass();

		~IBLRender();

	private:
		GLuint uniformEquTexture;
		GLuint uniformCubeMap;
		GLuint uniformIrradianceMap;

		glm::mat4 cubeTransMats[6];

	protected:
		IBLTexture *envCubeMap;
		IBLTexture *irradianceMap;
		Shader* equToCubeShader;
		Shader* convIrradianceShader;
		PostRenderHelper renderHelper;
		
		

};

