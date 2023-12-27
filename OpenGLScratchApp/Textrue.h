#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Textrue
{
public:
	Textrue();
	Textrue(const char* fileloc);
	Textrue(const char* filelocs[]);

	bool LoadTextrue();
	bool LoadTextrueAlpha();
	bool LoadCubeMap();

	void UseTextrue();
	void UseCubeMap();
	void ClearTextrue();

	GLuint getTextrueID();

	~Textrue();

private:
	GLuint textrueID;
	int width, height, bitDepth;

	const char* fileLocation;
	const char* cubeMapCollection[6];
};

