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
}


bool Textrue::LoadTextrue()
{
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}


bool Textrue::LoadTextrueAlpha()
{
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

void Textrue::UseTextrue()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textrueID);
}

void Textrue::ClearTextrue()
{
	glDeleteTextures(1, &textrueID);
	textrueID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}



Textrue::~Textrue()
{
	ClearTextrue();
}