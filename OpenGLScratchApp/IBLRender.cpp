#include "IBLRender.h"

IBLRender::IBLRender()
{
}

IBLRender::IBLRender(GLfloat textureWidth, GLfloat textureHeight)
{
	for (size_t i = 0; i < 6; i++)
	{
		
	}
	//envCubeMap = new IBLTexture();
	//envCubeMap->Init(textureWidth, textureHeight);

	renderHelper = PostRenderHelper();
	renderHelper.Init();

	equToCubeShader = new Shader();
	equToCubeShader->CreateFromFiles("Shaders/equ_to_cube.vert", "Shaders/equ_to_cube.frag");
	equToCubeShader->bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);

}

void IBLRender::EquirectangularToCubePass(Textrue EquirectangularTex)
{
	/*glm::mat4 model(1.0f);
	const float toRadians = 3.14159265f / 180.0f;


	model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0));
	
	
	

	equToCubeShader->UseShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EquirectangularTex.getTextrueID());

	uniformEquTexture = glGetUniformLocation(equToCubeShader->GetShaderID(), "equTexture");
	glUniform1i(uniformEquTexture, 0);

	//envCubeMap->BindFBO();

	for (unsigned int i = 0; i < 6; ++i)
	{
		envCubeMap->Write(i);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//renderHelper.GetFullquad()->RenderMesh();
	}*/

	for (unsigned int i = 0; i < 6; ++i)
	{

		glm::mat4 model(1.0f);
		const float toRadians = 3.14159265f / 180.0f;


		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0));

		equToCubeShader->UseShader();

		glUniformMatrix4fv(equToCubeShader->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));

		renderHelper.GetFullquad()->RenderMesh();

	}




}

void IBLRender::RenderIrradianceMapPass()
{
}

IBLRender::~IBLRender()
{
}
