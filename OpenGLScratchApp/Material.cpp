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

Material::Material(Shader* sourceShader)
{
	shader = sourceShader;
	this->GetAllProps();
}

void Material::GetAllProps()
{
	if (shader != nullptr)
	{
		GLuint shaderID = shader->GetShaderID();

		propNames.clear();
		propType.clear();
		textureNames.clear();
		textureLocations.clear();
		textureTypes.clear();

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

			if (values[1] == GL_SAMPLER_2D || values[1] == GL_SAMPLER_CUBE)
			{
				textureNames.push_back(name);
				textureLocations.push_back(attrib);
				textureTypes.push_back(values[1]);
			}
		}
	}
}

template<typename valueType>
void Material::SetPropValue(const char* propName, valueType value)
{
	std::vector<std::string>::iterator curPropName = std::find(propNames.begin(), propNames.end(), propName);

	if (curPropName != propNames.end())
	{
		GLuint curLocation = std::distance(propNames.begin(), curPropName);
		GLint curType = propType[curLocation];

		switch (curType)
		{
		case GL_FLOAT:
			glUniform1f(curLocation, value);
			break;
		case GL_FLOAT_VEC2:
			glUniform2f(curLocation, value.x, value.y);
			break;
		case GL_FLOAT_VEC3:
			glUniform3f(curLocation, value.x, value.y, value.z);
			break;
		case GL_FLOAT_VEC4:
			glUniform4f(curLocation, value.x, value.y, value.z, value.w);
			break;
		case GL_INT||GL_BOOL:
			glUniform1i(curLocation, value);
			break;
		default:
			printf("The property is not correct type!");
			break;
		}
	}
	else
	{
		printf("Property '%s' doesn't exist!", propName);

		return;
	}
}

void Material::AllocateTextures()
{
	if (textureNames.size() > 0)
	{
		for (size_t i = 0; i < textureNames.size(); i++)
		{
			glUniform1i(textureLocations[i], i);
		}
	}
}

void Material::SetTexture(const char* propName, Textrue tex)
{
	std::vector<std::string>::iterator curPropName = std::find(textureNames.begin(), textureNames.end(), propName);

	if (curPropName != propNames.end())
	{
		GLuint texUnit = std::distance(textureNames.begin(), curPropName);
		GLint curType = textureTypes[texUnit];

		glActiveTexture(GL_TEXTURE0 + texUnit);

		switch (curType)
		{
		case GL_SAMPLER_2D:
			glBindTexture(GL_SAMPLER_2D, tex.getTextrueID());
			break;
		case GL_SAMPLER_CUBE:
			glBindTexture(GL_SAMPLER_CUBE, tex.getTextrueID());
			break;
		default:
			printf("The texture is not correct type!");
			break;
		}
	}
	else
	{
		printf("Texture '%s' doesn't exist!", propName);

		return;
	}
	
}

void Material::UseMaterial()
{
	shader->UseShader();
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}



Material::~Material()
{

}