#pragma once

#include "Textrue.h"
#include "IBLTexture.h"
#include "Shader.h"
#include "PostRenderHelper.h"


class IBLRender
{
	public:
		IBLRender();
		IBLRender(GLfloat cubeWidth, GLfloat cubeHeight, GLfloat irraWidth, GLfloat irraHeight);
	
		IBLTexture* EquirectangularToCubePass(Textrue EquirectangularTex);
		IBLTexture* RenderIrradianceMapPass(GLuint envCubeMapID);
		IBLTexture* ImportanceSamplePass(GLuint envCubeMapID);
		IBLTexture* BRDFPreComputePass();


		~IBLRender();

	private:
		void CalTransformMatrix();

		GLuint uniformEquTexture;
		GLuint uniformCubeMap;
		GLuint uniformIrradianceMap;

		GLfloat rotateAngels[6] = { -90.0f, 90.0f, -90.0f, 90.0f, 0.0f, 180.0f };

		glm::vec3 rotateAxis[6] = { glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(1.0, 0.0, 0.0),
								  glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0) };

		glm::vec3 translateVectors[6] = { glm::vec3(1.0, 0.0, 0.0), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0),
										glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 0.0, 1.0) };

		glm::mat4 transMatrixs[6];


	protected:
		IBLTexture *envCubeMap;
		IBLTexture *irradianceMap;
		IBLTexture *importanceSampleMap;
		IBLTexture* brdfLUTMap;

		Shader* equToCubeShader;
		Shader* convIrradianceShader;
		Shader* importanceSampleShader;
		Shader* brdfPreComputeShader;

		PostRenderHelper renderHelper;
		
		

};

