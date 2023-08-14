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

#include "Model.h"

// Window dimensions
const float toRadians = 3.14159265f / 180.0f;


GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformViewPos = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader envMapShader;

Camera camera;

Textrue brickTextrue;
Textrue dirtTextrue;
Textrue PlainTextrue;
Textrue grassTextrue;
Textrue CubeMap;


Material shinyMaterial;
Material dullMaterial;
Material PlainMaterial;

Model xwing;
Model blackhawk;
Model ThirdOne;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint shader;


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
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f,

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

void createSkyBox()
{
	unsigned int cubeIndices[] =
	{
		//Top
	  0, 1, 2,
	  2, 3, 1,

	  //Bottom
	  4, 5, 6,
	  6, 7, 5,

	  //Front
	  8, 9, 10,
	  10, 11, 9,

	  //Back
	  12, 13, 14,
	  14, 15, 13,

	  //Left
	  16, 17, 18,
	  18, 19, 17,

	  //Right
	  20, 21, 22,
	  22, 23, 21

	};

	GLfloat cubeVertices[] =
	{
		//Top
	   -1, 1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//0
		1, 1, -1 , 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//1
	   - 1, 1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//2
		1, 1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//3

		//Bottom
		-1, -1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//4
		 1, -1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//5
		-1, -1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//6
		 1, -1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//7

		//Front
		- 1,  1, 1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//8
		 1,  1, 1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//9
		-1, -1, 1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//10
		 1, -1, 1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//11

		//Back
		-1,  1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//12
		 1,  1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//13
		-1, -1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//14
		 1, -1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//15

		//Left
		-1,  1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//16
		-1,  1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//17
		-1, -1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//18
		-1, -1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//19

		//Right
		1,  1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//20
		1,  1, -1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//21
		1, -1,  1, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//22
		1, -1, -1,  0.0f, 0.0f,	0.0f, 0.0f, 0.0f,//23
	};

	Mesh* skyBox = new Mesh();
	skyBox->CreateMesh(cubeVertices, cubeIndices, 192, 36);
	meshList.push_back(skyBox);

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



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

	envMapShader = Shader();
	envMapShader.CreateFromFiles("Shaders/enviroment_map.vert", "Shaders/enviroment_map.frag");

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
	//model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.0f));
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
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
	//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	ThirdOne.RenderModel();

}

void EnvMapPass(glm::mat4 P2WMat, glm::vec3 viewpos)
{
	glm::mat4 model(1.0f);
	envMapShader.UseShader();
	envMapShader.SetPtoWTransform(&P2WMat);
	envMapShader.SetViewPostion(&viewpos);

	glActiveTexture(GL_TEXTURE0);
	GLuint t_ID = CubeMap.getTextrueID();
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap.getTextrueID());

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformViewPos = shaderList[0].GetViewPosLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, 1366, 768);

	//clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformViewPos, camera.getCamPostion().x, camera.getCamPostion().y, camera.getCamPostion().z);

	shaderList[0].setDirectionalLight(&mainLight);
	shaderList[0].setPointLights(pointLights, pointLightCount);
	shaderList[0].setSpotLights(spotLights, spotLightCount);
	glm::mat4 mainLightTransMatrix = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&mainLightTransMatrix);

	mainLight.GetShadowMap()->Read(GL_TEXTURE1);
	shaderList[0].SetTexture(0);
	shaderList[0].SetDirectionalShadowMap(1);

	glm::vec3 lowerLight = camera.getCamPostion();
	lowerLight.y -= 0.2f;
	//spotLights[0].SetFlash(lowerLight, camera.getCamDirection());

	RenderScene();
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();


	CreateObjects();
	createEnvPlane();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTextrue = Textrue((char*)("Textures/brick.png"));
	brickTextrue.LoadTextrueAlpha();
	dirtTextrue = Textrue((char*)("Textures/BlacketTilling.png"));
	dirtTextrue.LoadTextrueAlpha();
	PlainTextrue = Textrue((char*)("Textures/plain.png"));
	PlainTextrue.LoadTextrueAlpha();
	grassTextrue = Textrue((char*)("Textures/GrassTilling.png"));
	grassTextrue.LoadTextrueAlpha();
	const char* cubeMapPath[6] = { "Textures/posx.jpg", "Textures/negx.jpg", "Textures/posy.jpg", "Textures/negy.jpg", "Textures/posz.jpg", "Textures/negz.jpg" };
	CubeMap = Textrue(cubeMapPath);
	CubeMap.LoadCubeMap();

	shinyMaterial = Material(1.0f, 32.0f);
	dullMaterial = Material(0.0f, 0.0f);
	PlainMaterial = Material(1.0f, 256.0f);

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	blackhawk = Model();
	blackhawk.LoadModel("Models/Sphere.obj");

	ThirdOne = Model();
	ThirdOne.LoadModel("Models/FinalBaseMesh.obj");


	mainLight = DirectionalLight(2048, 2048, 
		1.0f, 1.0f, 1.0f,
		0.1f, 0.6f,
		0.0f, -15.0f, -10.0f);


	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 1.8f,
		5.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-4.0f, 2.0f, 0.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;


	
	spotLights[0] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		50.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, -1.5f, 5.0f,
		0.0f, -1.0f, -100.0f,
		1.0f, 0.0f, 0.0f,
		50.0f);
	spotLightCount++;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	glm::mat4 inversPro = glm::inverse(projection);

	glm::mat4 PtoWMat = glm::mat4(inversPro);

	//loop until window close
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();//SDF_GetPerformanceCounter();
		deltaTime = now - lastTime;//(now - lastTime) * 1000 / SDF_GetPerformanceFrequency();
		lastTime = now;

		//Get and handle user input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		

		DirectinalShadowMapPass(&mainLight);
		RenderPass(projection, camera.calculateViewMatrix());

		PtoWMat = glm::mat4(glm::inverse(camera.calculateViewMatrix())) * glm::mat4(inversPro);
		
		EnvMapPass(PtoWMat, camera.getCamPostion());

		glUseProgram(0);

		mainWindow.swapBuffers();


	}

	return 0;
}