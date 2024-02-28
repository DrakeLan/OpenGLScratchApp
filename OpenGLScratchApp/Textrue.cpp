#include "Textrue.h"

Textrue::Textrue()
{
	textrueID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}

Textrue::Textrue(const char* fileloc)
{
	textrueID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileloc;

	for (size_t i = 0; i < 6; i++)
	{
		cubeMapCollection[i] = nullptr;
	}
}

Textrue::Textrue(const char* filelocs[])
{
	textrueID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
	
	for (int i = 0; i < 6; i++)
	{
		cubeMapCollection[i] = filelocs[i];
	}
}


bool Textrue::LoadTextrue(bool srgbFlag)
{
	if (srgbFlag)
	{
		texFormat = GL_SRGB;
	}
	else
	{
		texFormat = GL_RGB;
	}
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData)
	{
		printf("Filed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textrueID);
	glBindTexture(GL_TEXTURE_2D, textrueID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}


bool Textrue::LoadTextrueAlpha(bool srgbFlag)
{
	if (srgbFlag)
	{
		texFormat = GL_SRGB;
	}
	else
	{
		texFormat = GL_RGB;
	}

	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData)
	{
		printf("Filed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textrueID);
	glBindTexture(GL_TEXTURE_2D, textrueID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

bool Textrue::LoadTextrueHDR()
{

	float* texData = stbi_loadf(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData)
	{
		printf("Filed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textrueID);
	glBindTexture(GL_TEXTURE_2D, textrueID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, texData);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

bool Textrue::LoadCubeMap(bool srgbFlag)
{

	glGenTextures(1, &textrueID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textrueID);

	for (size_t faceCount = 0; faceCount < 6; faceCount++)
	{
		unsigned char *texData = stbi_load(cubeMapCollection[faceCount], &width, &height, &bitDepth, 0);

		if (!texData)
		{
			printf("Filed to find: %s\n", fileLocation);
			return false;
		}

		switch (faceCount)
		{

		case 0 :
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			break;

		case 1 :
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			break;

		case 2:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			break;

		case 3:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			break;

		case 4:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			break;

		case 5:
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
			break;

		}


		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
	


}


void Textrue::UseTextrue()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textrueID);
}

void Textrue::UseCubeMap()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textrueID);
}



void Textrue::ClearTextrue()
{
	glDeleteTextures(1, &textrueID);
	textrueID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;

	//for (size_t i = 0; i < 6; i++)
	//{
	//	delete cubeMapCollection[i];
	//}
}

GLuint Textrue::getTextrueID()
{
	return textrueID;
}



Textrue::~Textrue()
{
	ClearTextrue();
}