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
		MaterialProperty matProp = MaterialProperty();

		matProps.clear(); 
		matTextures.clear();

		std::vector<GLchar> nameData(256);
		std::vector<GLenum> properties;
		properties.push_back(GL_NAME_LENGTH);
		properties.push_back(GL_TYPE);
		
		std::vector<GLint> values(properties.size());

		glGetProgramInterfaceiv(shaderID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);


		for (int attrib = 0; attrib < numActiveUniforms; ++attrib)
		{
			glGetProgramResourceiv(shaderID, GL_UNIFORM, attrib, properties.size(), &properties[0], values.size(), NULL, &values[0]);

			nameData.resize(values[0]);
			glGetProgramResourceName(shaderID, GL_UNIFORM, attrib, nameData.size(), NULL, &nameData[0]);
			std::string name((char*)&nameData[0], nameData.size() - 1);

			matProp.propName = name;
			matProp.propLocation = attrib;
			matProp.propType = values[1];

			if (values[1] == GL_SAMPLER_2D || values[1] == GL_SAMPLER_CUBE)
			{
				matTextures.push_back(matProp);
			}
			else
			{
				matProps.push_back(matProp);
			}
		}
	}
}

MaterialProperty Material::GetMatProp(string propName)
{
	if (matProps.size() > 0)
	{
		for (size_t i = 0; i < matProps.size(); i++)
		{
			if (matProps[i].propName.compare(propName) == 0)
			{
				return matProps[i];
			}
			else
			{
				printf("Property isn't exsit!");
				return MaterialProperty();
			}
		}
	}
}

template<typename valueType>
void Material::SetPropValue(const char* propName, valueType value)
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

void Material::AllocateTextures()
{
	if (matTextures.size() > 0)
	{
		for (size_t i = 0; i < matTextures.size(); i++)
		{
			glUniform1i(matTextures[i].propLocation, i);
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