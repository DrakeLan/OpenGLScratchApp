#pragma once

#include <GL\glew.h>
#include "Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	Material(GLfloat sIntensity, GLfloat shine, Shader* shader);

	void GetAllProps();
	template<typename valueType>
	void SetPropValue(const char* propName, valueType value);
	void Debug();
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();


private:
	Shader* shader;
	GLfloat specularIntensity;
	GLfloat shininess;
	GLint numActiveUniforms = 0;
	std::vector<std::string> propNames;
	std::vector<GLint> propType;
	std::vector<GLint> propLoctations;
};

template<typename valueType>
inline void Material::SetPropValue(const char* propName, valueType value)
{
	std::vector<GLint>::iterator curPropName = std::find(propType.begin(), propType.end(), propName);

	if (curPropName != propType.end())
	{
		GLint curType = propType[std::distance(propType.begin(), curPropName)];
		switch (curType)
		{
		case GL_FLOAT:
			break;
		default:
			break;
		}
	}
	else
	{
		printf("Property '%s' doesn't exist!", propName);

		return;
	}
}
