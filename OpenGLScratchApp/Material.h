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

	template<typename... valueType>
	void SetPropValue(const char* propName, valueType ... value);

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
	std::vector<std::string> propNames;
	std::vector<GLint> propType;
	std::vector<std::string> textureNames;
	std::vector<GLuint> textureLocations;
	std::vector<GLuint> textureTypes;

	void GetAllProps();
	void AllocateTextures();
	void BindTextures();
	void SendValueToProgram();
	MaterialProperty GetMatProp(string propName);
};

template<typename... valueType>
inline void Material::SetPropValue(const char* propName, valueType ... value)
{
	MaterialProperty matProp = GetMatProp(propName);

	if (matProp.propName != "")
	{
		matProp.SetPropValue(value);
	}
	else
	{
		printf("Property '%s' doesn't exist!", propName);
	}

	return;
}

