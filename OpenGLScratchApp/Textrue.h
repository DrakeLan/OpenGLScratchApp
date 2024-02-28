#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Textrue
{
public:
	Textrue();
	Textrue(const char* fileloc);
	Textrue(const char* filelocs[]);

	bool LoadTextrue(bool srgbFlag = true);
	bool LoadTextrueAlpha(bool srgbFlag = true);
	bool LoadTextrueHDR();
	bool LoadCubeMap(bool srgbFlag = true);

	void UseTextrue();
	void UseCubeMap();
	void ClearTextrue();

	GLuint getTextrueID();

	~Textrue();

private:
	GLuint textrueID;
	GLenum texFormat;
	int width, height, bitDepth;

	const char* fileLocation;
	const char* cubeMapCollection[6];
};

