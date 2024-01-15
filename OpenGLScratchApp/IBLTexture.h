#pragma once

#include <stdio.h>
#include <GL\glew.h>
class IBLTexture
{

public:
	IBLTexture();

	virtual bool Init(GLuint width, GLuint height);

	virtual void Write();

	virtual void Read(GLenum textureUnit);

	GLuint GetTextureWidth() { return textureWidth; }
	GLuint GetTextureHeight() { return textureHeight; }


	~IBLTexture();

protected:
	GLuint FBO, textureID;
	GLuint textureWidth, textureHeight;

};

