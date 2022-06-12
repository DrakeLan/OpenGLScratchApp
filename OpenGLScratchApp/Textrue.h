#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Textrue
{
public:
	Textrue();
	Textrue(const char* fileloc);

	bool LoadTextrue();
	bool LoadTextrueAlpha();

	void UseTextrue();
	void ClearTextrue();

	~Textrue();

private:
	GLuint textrueID;
	int width, height, bitDepth;

	const char* fileLocation;
};

