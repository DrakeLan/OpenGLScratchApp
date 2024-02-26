#include "IBLRender.h"

IBLRender::IBLRender()
{
}

IBLRender::IBLRender(GLfloat cubeWidth, GLfloat cubeHeight, GLfloat irraWidth, GLfloat irraHeight)
{
	CalTransformMatrix();

	envCubeMap = new IBLTexture();
	envCubeMap->InitCube(cubeWidth, cubeHeight);

	irradianceMap = new IBLTexture();
	irradianceMap->InitCube(irraWidth, irraHeight);

	importanceSampleMap = new IBLTexture();
	importanceSampleMap->InitCube(irraWidth * 4, irraHeight * 4, true);

	brdfLUTMap = new IBLTexture();
	brdfLUTMap->InitLUT(512, 512);

	renderHelper = PostRenderHelper();
	renderHelper.Init();

	equToCubeShader = new Shader();
	equToCubeShader->CreateFromFiles("Shaders/equ_to_cube.vert", "Shaders/equ_to_cube.frag");

	convIrradianceShader = new Shader();
	convIrradianceShader->CreateFromFiles("Shaders/conv_to_irradiance.vert", "Shaders/conv_to_irradiance.frag");

	importanceSampleShader = new Shader();
	importanceSampleShader->CreateFromFiles("Shaders/importance_samplling.vert", "Shaders/importance_samplling.frag");

	brdfPreComputeShader = new Shader();
	brdfPreComputeShader->CreateFromFiles("Shaders/brdf_precompute.vert", "Shaders/brdf_precompute.frag");
}

void IBLRender::CalTransformMatrix()
{
	for (unsigned int i = 0; i < 6; ++i)
	{
		glm::mat4 model(1.0f);
		const float toRadians = 3.14159265f / 180.0f;

		model = glm::translate(model, translateVectors[i]);
		model = glm::rotate(model, rotateAngels[i] * toRadians, rotateAxis[i]);
		//model = glm::scale(model, glm::vec3(1.0, -1.0, 1.0));

		transMatrixs[i] = model;
	}

}

IBLTexture* IBLRender::EquirectangularToCubePass(Textrue EquirectangularTex)
{
	GLenum status;

	equToCubeShader->UseShader();

	envCubeMap->BindFBO();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	for (unsigned int i = 0; i < 6; ++i)
	{
		envCubeMap->Write(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, EquirectangularTex.getTextrueID());

		uniformEquTexture = glGetUniformLocation(equToCubeShader->GetShaderID(), "equTexture");
		glUniform1i(uniformEquTexture, 0);

		glUniformMatrix4fv(equToCubeShader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(transMatrixs[i]));

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Framebuffer Error: %i\n", status);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return NULL;
		}

		renderHelper.GetFullquad()->RenderMesh();

	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return envCubeMap;

}

IBLTexture* IBLRender::RenderIrradianceMapPass(GLuint envCubeMapID)
{
	GLenum status;

	convIrradianceShader->UseShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMapID);

	uniformCubeMap = glGetUniformLocation(convIrradianceShader->GetShaderID(), "envCubeMap");
	glUniform1i(uniformCubeMap, 0);

	irradianceMap->BindFBO();


	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceMap->Write(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(convIrradianceShader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(transMatrixs[i]));

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Framebuffer Error: %i\n", status);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return NULL;
		}

		renderHelper.GetFullquad()->RenderMesh();

	}

	return irradianceMap;
}

IBLTexture* IBLRender::ImportanceSamplePass(GLuint envCubeMapID)
{
	importanceSampleShader->UseShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMapID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


	uniformCubeMap = glGetUniformLocation(convIrradianceShader->GetShaderID(), "envCubeMap");
	glUniform1i(uniformCubeMap, 0);

	importanceSampleMap->BindFBO();

	unsigned int maxMipLevels = 5;

	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		importanceSampleShader->SetFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glUniformMatrix4fv(importanceSampleShader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(transMatrixs[i]));
			importanceSampleMap->Write(i, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderHelper.GetFullquad()->RenderMesh();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return importanceSampleMap;
}


IBLTexture* IBLRender::BRDFPreComputePass()
{
	brdfPreComputeShader->UseShader();
	brdfLUTMap->BindFBO();
	brdfLUTMap->Write();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderHelper.GetFullquad()->RenderMesh();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return brdfLUTMap;

}



IBLRender::~IBLRender()
{
}
