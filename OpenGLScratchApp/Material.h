#pragma once

#include <GL\glew.h>
#include "Shader.h"
#include "Textrue.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	Material(GLfloat sIntensity, GLfloat shine, Shader* shader);
	Material(Shader* sourceShader);

	void GetAllProps();
	template<typename valueType>
	void SetPropValue(const char* propName, valueType value);
	void AllocateTextures();
	void SetTexture(const char* propName, Textrue tex);
	void UseMaterial();
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();


private:
	Shader* shader;
	GLfloat specularIntensity;
	GLfloat shininess;
	GLint numActiveUniforms = 0;
	std::vector<std::string> propNames;
	std::vector<GLint> propType;
	std::vector<std::string> textureNames;
	std::vector<GLuint> textureLocations;
	std::vector<GLuint> textureTypes;
};


