#include "IBLRender.h"

IBLRender::IBLRender()
{
}

IBLRender::IBLRender(GLfloat textureWidth, GLfloat textureHeight)
{

	envCubeMap = new IBLTexture();
	envCubeMap->Init(textureWidth, textureHeight);

	renderHelper = PostRenderHelper();
	renderHelper.Init();

	equToCubeShader = new Shader();
	equToCubeShader->CreateFromFiles("Shaders/equ_to_cube.vert", "Shaders/equ_to_cube.frag");
	equToCubeShader->bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);

}

IBLTexture* IBLRender::EquirectangularToCubePass(Textrue EquirectangularTex)
{
	
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error: %i\n", status);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return NULL;
	}

	equToCubeShader->UseShader();

	envCubeMap->BindFBO();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for (unsigned int i = 0; i < 6; ++i)
	{

		glm::mat4 model(1.0f);
		const float toRadians = 3.14159265f / 180.0f;

		model = glm::translate(model, translateVectors[i]);
		model = glm::rotate(model, rotateAngels[i] * toRadians, rotateAxis[i]);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap->GetTextureID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, EquirectangularTex.getTextrueID());

		uniformEquTexture = glGetUniformLocation(equToCubeShader->GetShaderID(), "equTexture");
		glUniform1i(uniformEquTexture, 0);

		glUniformMatrix4fv(equToCubeShader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));

		renderHelper.GetFullquad()->RenderMesh();

	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return envCubeMap;

}

void IBLRender::RenderIrradianceMapPass()
{
}

IBLRender::~IBLRender()
{
}
