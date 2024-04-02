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
			matProp.propLocation = glGetUniformLocation(shaderID, name.c_str());
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

MaterialProperty* Material::GetMatProp(string propName, vector<MaterialProperty>* props)
{
	if (props->size() > 0)
	{
		for (size_t i = 0; i < props->size(); i++)
		{
			if (props->at(i).propName.compare(propName) == 0)
			{
				return &(props->at(i));
			}
		}
		return  nullptr;
	}
}



void Material::AllocateTextures()
{
	if (matTextures.size() > 0)
	{
		
		for (size_t i = 0; i < matTextures.size(); i++)
		{
			if (matTextures[i].dirtyFlag)
			{
				glUniform1i(matTextures[i].propLocation, i);
				matTextures[i].dirtyFlag = false;
			}
		}
	}
}

void Material::BindTextures()
{
	for (size_t i = 0; i < matTextures.size(); i++)
	{
		if (matTextures[i].propValue.size() > 0)
		{

			glActiveTexture(GL_TEXTURE0 + i);
			switch (matTextures[i].propType)
			{
			case GL_SAMPLER_2D:
				glBindTexture(GL_TEXTURE_2D, (GLuint)(matTextures[i].propValue[0]));
				break;
			case GL_SAMPLER_CUBE:
				glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)(matTextures[i].propValue[0]));
				break;
			default:
				printf("The texture is not correct type!");
				break;
			}

		}
		else
		{
			printf("Texture is not set!");
		}

	}
}

void Material::SendValueToProgram()
{
	if (matProps.size() > 0)
	{
		for (size_t i = 0; i < matProps.size(); i++)
		{
			if (matProps[i].dirtyFlag)
			{
				switch (matProps[i].propType)
				{
				case GL_FLOAT:
					glUniform1f(matProps[i].propLocation, matProps[i].propValue[0]);
					break;
				case GL_FLOAT_VEC2:
					glUniform2f(matProps[i].propLocation, matProps[i].propValue[0], matProps[i].propValue[1]);
					break;
				case GL_FLOAT_VEC3:
					glUniform3f(matProps[i].propLocation, matProps[i].propValue[0], matProps[i].propValue[1], matProps[i].propValue[2]);
					break;
				case GL_FLOAT_VEC4:
					glUniform4f(matProps[i].propLocation, matProps[i].propValue[0], matProps[i].propValue[1], matProps[i].propValue[2], matProps[i].propValue[3]);
					break;
				case GL_INT || GL_BOOL:
					glUniform1i(matProps[i].propLocation, matProps[i].propValue[0]);
					break;
				default:
					printf("The property is not correct type!");
					break;
				}
				matProps[i].dirtyFlag = false;
			}

		}
	}
}


void Material::SetPropValue(const char* propName, float x, float y, float z, float w)
{
	MaterialProperty* matprop = GetMatProp(propName, &matProps);

	if (matprop != nullptr)
	{
		matprop->SetPropValue(x, y, z, w);
	}
	else
	{
		printf("Property '%s' doesn't exist!", propName);
	}

	return;
}

void Material::SetTextureValue(const char* propName, float x)
{
	MaterialProperty* matprop = GetMatProp(propName, &matTextures);

	if (matprop != nullptr)
	{
		matprop->SetPropValue(x);
	}
	else
	{
		printf("Texture '%s' doesn't exist!", propName);
	}

	return;
}

void Material::SetShader(Shader sourceShader)
{
	shader = &sourceShader;
	GetAllProps();
}

void Material::UseMaterial()
{
	shader->UseShader();

	
	BindTextures();
	AllocateTextures();
	SendValueToProgram();
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}



Material::~Material()
{

}