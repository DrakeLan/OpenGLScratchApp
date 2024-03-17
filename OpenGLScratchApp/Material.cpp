#include "Material.h"


Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

Material::Material(GLfloat sIntensity, GLfloat shine, Shader* sourceShader)
{
	specularIntensity = sIntensity;
	shininess = shine;
	shader = sourceShader;
}

void Material::GetAllProps()
{
	if (shader != nullptr)
	{
		GLuint shaderID = shader->GetShaderID();

		propNames.clear();
		propType.clear();

		std::vector<GLchar> nameData(256);
		std::vector<GLenum> properties;
		properties.push_back(GL_NAME_LENGTH);
		properties.push_back(GL_TYPE);
		
		std::vector<GLint> values(properties.size());


		glGetProgramInterfaceiv(shaderID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);
		propNames.resize(numActiveUniforms);
		propType.resize(numActiveUniforms);

		for (int attrib = 0; attrib < numActiveUniforms; ++attrib)
		{
			glGetProgramResourceiv(shaderID, GL_UNIFORM, attrib, properties.size(), &properties[0], values.size(), NULL, &values[0]);

			nameData.resize(values[0]);
			glGetProgramResourceName(shaderID, GL_UNIFORM, attrib, nameData.size(), NULL, &nameData[0]);
			std::string name((char*)&nameData[0], nameData.size() - 1);
			propNames[attrib] = name;
			propType[attrib] = values[1];
		}
	}
}

void Material::Debug()
{
	if(propType.size() > 0)
	for (size_t i = 0; i < propType.size(); i++)
	{
		if (propType[i] == GL_FLOAT)
		{
			std::cout << "float";
		}
		else
		{
			std::cout << "others";
		}
	}
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}



Material::~Material()
{

}