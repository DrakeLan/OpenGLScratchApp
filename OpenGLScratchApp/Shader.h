#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFilesWithGeo(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	void CreateFromFilesWithTess(const char* vertexLocation, const char* tessControlLocation, const char* tessEvaluateLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	//To Do: Simplify in Getxxx() function
	GLuint GetShaderID();
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetOmniLightFarPlaneLocation();
	GLuint GetTessParamLocation();
	GLuint GetTessHeightLocation();
	GLuint GetDebugFlagLocation();

	//To Do: Simplify in Setxxx() function
	void setDirectionalLight(DirectionalLight * dLight);
	void setPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void setSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetOmniLightMatrices(std::vector<glm::mat4>lightMatrices);

	//General set function
	void SetModel(glm::mat4* value);

	void SetFloat(const char* propertyName, GLfloat value);
	void SetVectorThree(const char* propertyName, glm::vec3* value);
	void SetMatrix(const char* propertyName, glm::mat4* value);
	void SetTexture(const char* textureName, GLuint value);

	void bindUniformBlockToBindingPoint(const std::string& uniformBlockName, const GLuint bindingPoint);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformOmniLightFarPlane,
		uniformTessParam, uniformTessHeight, uniformDebugFlag;

	GLuint uniformOmniLightMatrices[6];


	struct directionLightData
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;

	};
	struct directionLightData uniformDirectionLight;


	GLuint uniformPointLightCount;

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

	}uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct 
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	}uniformSpotLight[MAX_SPOT_LIGHTS];

	struct
	{
		GLuint shadowMap;
		GLuint farPlane;

	}uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShaderWithGeo(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void CompileShaderWithTess(const char* vertexCode, const char* tessControlCode, const char* tessEvaluateLocation, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum saderType);

	void CompileProgram();
};

