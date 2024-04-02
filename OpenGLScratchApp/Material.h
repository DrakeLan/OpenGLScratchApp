#pragma once

#include "MaterialProperty.h"
#include "Shader.h"
#include "Textrue.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	Material(Shader* sourceShader);

	void SetPropValue(const char* propName, float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	void SetTextureValue(const char* propName, float x = 0.0f);

	void SetShader(Shader sourceShader);
	void UseMaterial();
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();


private:
	Shader* shader;
	GLfloat specularIntensity;
	GLfloat shininess;
	GLint numActiveUniforms = 0;
	vector<MaterialProperty> matProps;
	vector<MaterialProperty> matTextures;


	void GetAllProps();
	void AllocateTextures();
	void BindTextures();
	void SendValueToProgram();
	MaterialProperty* GetMatProp(string propName, vector<MaterialProperty>* props);
};

