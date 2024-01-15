#include "IBLRender.h"

IBLRender::IBLRender()
{
}

IBLRender::IBLRender(GLfloat textureWidth, GLfloat textureHeight)
{
	envCubeMap = new IBLTexture();
	envCubeMap->Init(textureWidth, textureHeight);
}

void IBLRender::EquirectangularToCube(Textrue EquirectangularTex)
{
	etangToCubeShader->UseShader();

	envCubeMap->Write();
	glClear(GL_COLOR_CLEAR_VALUE);

}
