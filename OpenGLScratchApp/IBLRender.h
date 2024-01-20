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
		
		IBLTexture* EquirectangularToCubePass(Textrue EquirectangularTex);
		void RenderIrradianceMapPass();

		~IBLRender();

	private:
		GLuint uniformEquTexture;
		GLuint uniformCubeMap;
		GLuint uniformIrradianceMap;

		GLfloat rotateAngels[6] = { -90.0f, 90.0f, -90.0f, 90.0f, 0.0f, 180.0f };

		glm::vec3 rotateAxis[6] = { glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 0.0, 0.0),
								  glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0) };

		glm::vec3 translateVectors[6] = { glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0),
										glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 0.0, 1.0) };




	protected:
		IBLTexture *envCubeMap;
		IBLTexture *irradianceMap;
		Shader* equToCubeShader;
		Shader* convIrradianceShader;
		PostRenderHelper renderHelper;
		
		

};

