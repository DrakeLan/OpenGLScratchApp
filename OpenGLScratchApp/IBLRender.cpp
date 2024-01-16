#include "IBLRender.h"

IBLRender::IBLRender()
{
}

IBLRender::IBLRender(GLfloat textureWidth, GLfloat textureHeight)
{
	envCubeMap = new IBLTexture();
	envCubeMap->Init(textureWidth, textureHeight);

	renderHelper = new PostRenderHelper();
}

void IBLRender::EquirectangularToCube(Textrue EquirectangularTex)
{
	etangToCubeShader->UseShader();

	envCubeMap->Write();
	glClear(GL_COLOR_CLEAR_VALUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EquirectangularTex.getTextrueID());

	glUniform1i("EquirectangularTexture", 0);


	renderHelper->RenderMesh();


}
