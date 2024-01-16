#include "IBLRender.h"

IBLRender::IBLRender()
{
}

IBLRender::IBLRender(GLfloat textureWidth, GLfloat textureHeight)
{
	envCubeMap = new IBLTexture();
	envCubeMap->Init(textureWidth, textureHeight);

	renderHelper = PostRenderHelper();

	equToCubeShader = new Shader();
	equToCubeShader->CreateFromFilesWithGeo("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geo", "Shaders/omni_shadow_map.frag");

}

void IBLRender::EquirectangularToCube(Textrue EquirectangularTex)
{
	equToCubeShader->UseShader();

	envCubeMap->Write();
	glClear(GL_COLOR_CLEAR_VALUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EquirectangularTex.getTextrueID());

	uniformEquTexture = glGetUniformLocation(equToCubeShader->GetShaderID(), "equTexture");
	glUniform1i(uniformEquTexture, 0);


	renderHelper.GetFullquad()->RenderMesh();


}
