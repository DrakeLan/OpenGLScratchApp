#pragma once

#include <stdio.h>
#include <GL\glew.h>
class IBLTexture
{

public:
	IBLTexture();

	bool Init(GLuint width, GLuint height);

	void BindFBO();

	void Write(GLuint i);

	void Read(GLenum textureUnit);

	GLuint GetTextureWidth() { return textureWidth; }
	GLuint GetTextureHeight() { return textureHeight; }
	GLuint GetTextureID() { return textureID; }


	~IBLTexture();

protected:
	GLuint FBO, textureID;
	GLuint textureWidth, textureHeight;

};

