#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Textrue.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "UniformBufferObject.h"
#include "Model.h"
#include "RenderTexture.h"
#include "IBLRender.h"

// Window dimensions
const GLuint windowWidth = 1366;
const GLuint windowHeight = 768;

const float toRadians = 3.14159265f / 180.0f;

glm::vec3 cameraPos;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformOmniFarPlane = 0, g_GlobalMatricesUBO = 0,
uniformTessParam = 0, uniformTessHieght = 0, uniformDebugFlag = 0;

GLuint uniformMetallic = 0, uniformRoughness = 0, uniformAO = 0;

UniformBufferObject globalMatrixUBO;
UniformBufferObject instancingMatrixUBO;
UniformBufferObject instancingColorUBO;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
Shader envMapShader;
Shader reflectionShader;
Shader distortionShader;

Shader basicTessellationShader;

Shader basicInstancingShader;

Shader basicPBRShader;

Shader copyShader;


Camera camera;

Textrue brickTextrue;
Textrue dirtTextrue;
Textrue PlainTextrue;
Textrue grassTextrue;
Textrue heightTextrue;
Textrue CubeMap;
Textrue iblRadianceTexture;


Material shinyMaterial;
Material dullMaterial;
Material PlainMaterial;
Material pbrMaterial;

Model xwing;
Model blackhawk;
Model ThirdOne;
Model TeaPot;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat tessParam = 16.0f;
GLfloat tessHeight = 1.0f;
GLfloat debugFlag = 0.0f;

GLuint shader;

glm::mat4 instancingMatrics[1000];
glm::vec4 instancingColor[1000];

IBLRender iblRender;
IBLTexture *envCubeMap;
IBLTexture *irradianceTexture;
IBLTexture *importanceSampleTexture;
IBLTexture *brdfPreComputeMap;

RenderTexture baseRT;

//To do: create a OP class to control, do not create a flag every frame
bool pressedFlag = false;


// Vertex Shader code
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calAverageNormal(unsigned int * indices, unsigned int indicecount, GLfloat * vertices, unsigned int verticecount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indicecount; i+=3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1+1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticecount/ vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = 
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = 
	{
		//x      y     z	 u     v		normal xyz
		-1.0f, -1.0f, -0.6f,	 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	 0.5f, 0.0f,	0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,	 1.0f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	 0.5f, 1.0f,	0.0f, 0.0f, 0.0f,
	};

	unsigned int floorIndices[] = 
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] =
	{
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,	0.0f, 1.0f, 0.0f,

	};

	calAverageNormal(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

}



void createEnvPlane()
{
	unsigned int clipIndcies[]=
	{
		0, 2, 1,
		2, 3, 1
	};

	GLfloat clipVertices[] = 
	{
		-1.0f, 1.0f, 0.999f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
		1.0, 1.0, 0.999,      1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
		-1.0, -1.0, 0.999,    0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		1.0, -1.0, 0.999,     1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	};

	Mesh *clipObj = new Mesh();
	clipObj->CreateMesh(clipVertices, clipIndcies, 32, 6);
	meshList.push_back(clipObj);
}

void CreateGlobalMatrixUBO()
{
	globalMatrixUBO = UniformBufferObject();
	globalMatrixUBO.createUBO(sizeof(glm::mat4) * 2, GL_STREAM_DRAW);
	globalMatrixUBO.bindBufferBaseToBindingPoint(MATRICES_BLOCK_BINDING_POINT);

}

void CreateInstancingUBO(const int amounts)
{
	float counts = sizeof(instancingMatrics) / sizeof(instancingMatrics[0]);

	//Every UBO has a maxium size, so matrix and color slpit in two UBO, GL_MAX_UNIFORM_BLOCK_SIZE to check

	instancingMatrixUBO = UniformBufferObject();
	instancingMatrixUBO.createUBO(sizeof(glm::mat4) * counts, GL_STREAM_DRAW);

	instancingColorUBO = UniformBufferObject();
	instancingColorUBO.createUBO(sizeof(glm::vec4) * counts, GL_STREAM_DRAW);

	glm::mat4 model(1.0f);
	float offset = 5.0f;
	float radius = 10.0f;

	float randColR = 0.0;
	float randColG = 0.0;
	float randColB = 0.0;

	int spacingCounts = 0;



	for (float x = 0.0f; x < 10.0f; x++)
	{
		for (float y = 0.0f; y < 10.0f; y++)
		{
			for (float z = 0.0f; z < 10.0; z++)
			{

				model = glm::mat4(1.0f);
				//model = glm::scale(model, glm::vec3(1.0));
				model = glm::translate(model, (glm::vec3(x - 4.5f, y - 4.5f, z - 4.5f) * 5.0f));
				
				if (spacingCounts < counts)
				{
					instancingMatrics[spacingCounts] = model;

					randColR = (rand() % 100) / 100.0f;
					randColG = (rand() % 100) / 100.0f;
					randColB = (rand() % 100) / 100.0f;

					instancingColor[spacingCounts] = glm::vec4(randColR, randColG, randColB, 1.0);


					spacingCounts++;
				}
			}
		}

	}
		

	instancingMatrixUBO.setBufferData(0.0, instancingMatrics, sizeof(instancingMatrics));
	instancingMatrixUBO.bindBufferBaseToBindingPoint(INSTANCING_BLOCK_BINDING_POINT_ONE);

	instancingColorUBO.setBufferData(0.0, &instancingColor, sizeof(instancingColor));
	instancingColorUBO.bindBufferBaseToBindingPoint(INSTANCING_BLOCK_BINDING_POINT_TWO);
}

void SetGlobalMatrixUBO(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	//std::cout << glGetError() << std::endl; //Drop 1282 every frame after first call, why?
	globalMatrixUBO.setBufferData(0, glm::value_ptr(projectionMatrix), sizeof(glm::mat4));
	globalMatrixUBO.setBufferData(sizeof(glm::mat4), glm::value_ptr(viewMatrix), sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CreatBaseRenderTarget(GLuint targetWidth, GLuint targetHeight)
{
	baseRT.Init_SRGBA(targetWidth, targetHeight);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shader1->bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

	omniShadowShader = Shader();
	omniShadowShader.CreateFromFilesWithGeo("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geo", "Shaders/omni_shadow_map.frag");

	basicTessellationShader = Shader();
	basicTessellationShader.CreateFromFilesWithTess("Shaders/basic_tessellation.vert", "Shaders/basic_tessellation_control.tesscon", "Shaders/basic_tessellation_evaluate.tesseva", "Shaders/basic_tessellation.frag");
	basicTessellationShader.bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);

	basicInstancingShader = Shader();
	basicInstancingShader.CreateFromFiles("Shaders/basic_instancing.vert", "Shaders/basic_instancing.frag");
	basicInstancingShader.bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);
	basicInstancingShader.bindUniformBlockToBindingPoint("instancingMatrixBlock", INSTANCING_BLOCK_BINDING_POINT_ONE);
	basicInstancingShader.bindUniformBlockToBindingPoint("instancingColorBlock", INSTANCING_BLOCK_BINDING_POINT_TWO);

	basicPBRShader = Shader();
	basicPBRShader.CreateFromFiles("Shaders/basic_PBR.vert", "Shaders/basic_PBR.frag");
	basicPBRShader.bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);

	envMapShader = Shader();
	envMapShader.CreateFromFiles("Shaders/enviroment_map.vert", "Shaders/enviroment_map.frag");

	reflectionShader = Shader();
	reflectionShader.CreateFromFiles("Shaders/reflection.vert", "Shaders/reflection.frag");
	reflectionShader.bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);

	distortionShader = Shader();
	distortionShader.CreateFromFiles("Shaders/reflection.vert", "Shaders/reflection_distortion.frag");
	distortionShader.bindUniformBlockToBindingPoint("globalMatrixBlock", MATRICES_BLOCK_BINDING_POINT);

	copyShader = Shader();
	copyShader.CreateFromFiles("Shaders/copy_shader.vert", "Shaders/copy_shader.frag");

}

void RenderScene()
{
	glm::mat4 model(1.0f);

	//TRS
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTextrue.UseTextrue();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTextrue.UseTextrue();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));//
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	PlainTextrue.UseTextrue();
	PlainMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTextrue.UseTextrue();
	PlainMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	xwing.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));//1.0
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
	//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.0f, 0.0f, 0.0f));//0.3
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	ThirdOne.RenderModel();

}

void InitTessParams()
{
	uniformTessParam = basicTessellationShader.GetTessParamLocation();
	uniformTessHieght = basicTessellationShader.GetTessHeightLocation();
	uniformDebugFlag = basicTessellationShader.GetDebugFlagLocation();

}


void TessellationOp(bool* keys)
{

	if (keys[GLFW_KEY_RIGHT])
	{
		if (tessParam <= 128.0f)
		{
			tessParam += 1.0;
			if (tessParam > 120.0f)
			{
				tessParam = 120.0f;
			}
		}
	
	}

	if (keys[GLFW_KEY_LEFT])
	{
		if (tessParam >= 1.0f)
		{
			tessParam -= 1.0f;
			if (tessParam < 1.0f)
			{
				tessParam = 1.0f;
			}
		}
	}

	if (keys[GLFW_KEY_UP])
	{
		if (tessHeight <= 5.0f)
		{
			tessHeight += 0.1;
			if (tessHeight > 5.0f)
			{
				tessHeight = 5.0f;
			}
		}
	}

	if (keys[GLFW_KEY_DOWN])
	{
		if (tessHeight >= 0.0f)
		{
			tessHeight -= 0.1f;
			if (tessHeight < 0.0f)
			{
				tessHeight = 0.0f;
			}
		}
	}

	if (keys[GLFW_KEY_N])
	{
		if (!pressedFlag)
		{
			if (debugFlag > 0.5f)
			{
				debugFlag = 0.0f;
			}
			else
			{
				debugFlag = 1.0f;
			}

			pressedFlag = true;

		}

	}
	else
	{
		pressedFlag = false;
	}
}

void ClearPass()
{
	glViewport(0, 0, windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	baseRT.Clear();
}

void OnScreenPass()
{

	PostRenderHelper renderHelper = PostRenderHelper();
	renderHelper.Init();

	glViewport(0, 0, windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	copyShader.UseShader();
	baseRT.Read(GL_TEXTURE0);

	copyShader.SetTexture("sourceTex", 0);

	renderHelper.GetFullquad()->RenderMesh();

}

void EnvMapPass(glm::mat4 P2WMat)
{
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	envMapShader.UseShader();
	envMapShader.SetMatrix("PToWTransform", &P2WMat);

	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap->GetTextureID());

	meshList[3]->RenderMesh();

}

void DirectinalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 LightTransMatrix = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&LightTransMatrix);

	RenderScene();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformOmniFarPlane = omniShadowShader.GetOmniLightFarPlaneLocation();
	
	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformOmniFarPlane, light->GetFarPlane());
	omniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());

	RenderScene();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ReflectionObjPass()
{
	glm::mat4 model(1.0f);

	reflectionShader.UseShader();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	uniformModel = reflectionShader.GetModelLocation();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	reflectionShader.SetVectorThree("viewPosition", &cameraPos);

	//CubeMap.UseCubeMap();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, importanceSampleTexture->GetTextureID());

	blackhawk.RenderModel();

	distortionShader.UseShader();
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	uniformModel = distortionShader.GetModelLocation();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	distortionShader.SetVectorThree("viewPosition", &cameraPos);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[2]->RenderMesh();

}

void TessellationObjectPass(GLfloat tessParam, GLfloat tessHeight)
{
	glm::mat4 model(1.0f);

	basicTessellationShader.UseShader();

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	uniformModel = basicTessellationShader.GetModelLocation();


	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	
	glUniform1f(uniformDebugFlag, 0.0f);
	glUniform1f(uniformTessParam, tessParam);
	glUniform1f(uniformTessHieght, tessHeight);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, heightTextrue.getTextrueID());

	meshList[2]->RenderMeshasPatch();

	if (debugFlag > 0.5)
	{
		glUniform1f(uniformDebugFlag, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		meshList[2]->RenderMeshasPatch();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


}

void InstancingPass()
{
	basicInstancingShader.UseShader();

	blackhawk.RenderModelInstancing(1000);

}

//To DO: Standarlize scene render pass
void PBRPass()
{
	pbrMaterial.UseMaterial();
	
	uniformModel = basicPBRShader.GetModelLocation();
	
	basicPBRShader.SetVectorThree("viewPosition", &cameraPos);

//To Do: Make light data in UBO
	basicPBRShader.setDirectionalLight(&mainLight);

	glm::mat4 model(1.0f);

	//TRS
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	
	//meshList[2]->RenderMesh();
	blackhawk.RenderModel();

}

void RenderPass()
{
	shaderList[0].UseShader();
	//TO DO: Use materials to query uniform varible need to be full in different shader
	uniformModel = shaderList[0].GetModelLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();


	shaderList[0].SetVectorThree("viewPosition", &cameraPos);

	shaderList[0].setDirectionalLight(&mainLight);
	shaderList[0].setPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, 0 + pointLightCount);
	glm::mat4 mainLightTransMatrix = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&mainLightTransMatrix);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCamPostion();
	lowerLight.y -= 0.2f;
	//spotLights[0].SetFlash(lowerLight, camera.getCamDirection());

	RenderScene();
}

int main()
{
	mainWindow = Window(windowWidth, windowHeight);
	mainWindow.Initialise();

	CreateObjects();
	createEnvPlane();

	CreateGlobalMatrixUBO();

	CreateInstancingUBO(1000);

	CreateShaders();

	InitTessParams();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTextrue = Textrue((char*)("Textures/brick.png"));
	brickTextrue.LoadTextrueAlpha();
	dirtTextrue = Textrue((char*)("Textures/BlacketTilling.png"));
	dirtTextrue.LoadTextrueAlpha();
	PlainTextrue = Textrue((char*)("Textures/plain.png"));
	PlainTextrue.LoadTextrueAlpha();
	grassTextrue = Textrue((char*)("Textures/GrassTilling.png"));
	grassTextrue.LoadTextrueAlpha();
	heightTextrue = Textrue((char*)("Textures/teapot_disp.png"));
	heightTextrue.LoadTextrue();
	iblRadianceTexture = Textrue((char*)("Textures/table_mountain_1_puresky_4k.hdr"));
	iblRadianceTexture.LoadTextrueHDR();
	const char* cubeMapPath[6] = { "Textures/posx.jpg", "Textures/negx.jpg", "Textures/posy.jpg", "Textures/negy.jpg", "Textures/posz.jpg", "Textures/negz.jpg" };
	CubeMap = Textrue(cubeMapPath);
	CubeMap.LoadCubeMap();

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	blackhawk = Model();
	blackhawk.LoadModel("Models/Sphere.obj");

	ThirdOne = Model();
	ThirdOne.LoadModel("Models/FinalBaseMesh.obj");

	TeaPot = Model();
	TeaPot.LoadModel("Models/teapot.obj");


	mainLight = DirectionalLight(2048, 2048, 
		1.0f, 1.0f, 1.0f,
		0.1f, 0.6f,
		0.0f, -15.0f, -10.0f);


	pointLights[0] = PointLight(1024,1024, 
		0.01f, 100.0f, 
		0.0f, 0.0f, 1.0f,
		0.0f, 1.8f,
		5.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(
		1024, 1024,
		0.01f, 100.0f, 
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-4.0f, 2.0f, 0.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;


	
	spotLights[0] = SpotLight(1024, 1024,
		0.01f, 100.0f, 
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		50.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
		0.01f, 100.0f, 
		1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, -1.5f, 5.0f,
		0.0f, -1.0f, -100.0f,
		1.0f, 0.0f, 0.0f,
		50.0f);
	spotLightCount++;

	cameraPos = camera.getCamPostion();

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	glm::mat4 inversPro = glm::inverse(projection);

	glm::mat4 PtoWMat = glm::mat4(inversPro);

	glm::vec4 testColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	iblRender = IBLRender(512.0f, 512.0f, 32.0f, 32.0f);
	envCubeMap = new IBLTexture();
	envCubeMap = iblRender.EquirectangularToCubePass(iblRadianceTexture);
	irradianceTexture = new IBLTexture();
	irradianceTexture = iblRender.RenderIrradianceMapPass(envCubeMap->GetTextureID());
	importanceSampleTexture = new IBLTexture();
	importanceSampleTexture = iblRender.ImportanceSamplePass(envCubeMap->GetTextureID());
	brdfPreComputeMap = new IBLTexture();
	brdfPreComputeMap = iblRender.BRDFPreComputePass();

	shinyMaterial = Material(1.0f, 32.0f);
	dullMaterial = Material(0.0f, 0.0f);
	PlainMaterial = Material(1.0f, 256.0f);

	pbrMaterial = Material(&basicPBRShader);
	pbrMaterial.SetPropValue("metallic", 1.0f);
	pbrMaterial.SetPropValue("roughness", 0.0f);
	pbrMaterial.SetPropValue("ao", 1.0f);
	pbrMaterial.SetTextureValue("irradianceMap", irradianceTexture->GetTextureID());
	pbrMaterial.SetTextureValue("importanceSampleMap", importanceSampleTexture->GetTextureID());
	pbrMaterial.SetTextureValue("BRDF_LUT", brdfPreComputeMap->GetTextureID());

	CreatBaseRenderTarget(windowWidth, windowHeight);
	
	//loop until window close
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();//SDF_GetPerformanceCounter();
		deltaTime = now - lastTime;//(now - lastTime) * 1000 / SDF_GetPerformanceFrequency();
		lastTime = now;

		//Get and handle user input events
		glfwPollEvents();

		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		camera.KeyControl(mainWindow.getsKeys(), deltaTime);


		//Start render
		ClearPass();
		baseRT.BindFBO();
		baseRT.Write();

		cameraPos = camera.getCamPostion();

		SetGlobalMatrixUBO(projection, camera.calculateViewMatrix());

		DirectinalShadowMapPass(&mainLight);

		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}

		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}

		baseRT.BindFBO();
		baseRT.Write();
		RenderPass();
		//ReflectionObjPass();
		//TessellationOp(mainWindow.getsKeys());
		//TessellationObjectPass(tessParam, tessHeight);
		PBRPass();
		//InstancingPass();

		PtoWMat = glm::mat4(glm::inverse(camera.calculateOriginalViewMatrix())) * glm::mat4(inversPro);
		
		EnvMapPass(PtoWMat);

		OnScreenPass();

		glUseProgram(0);

		mainWindow.swapBuffers();


	}

	return 0;
}