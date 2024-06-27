#pragma once

#include <stdio.h>
#include <GL\glew.h>
class IBLTexture
{

public:
	IBLTexture();

	bool InitCube(GLuint width, GLuint height, bool mipFlag = false, bool HDRFlag = false);
	bool InitLUT(GLuint width, GLuint height);

	void BindFBO();

	void Write();

	void Write(GLuint i, GLuint mip = 0);

	void Read(GLenum textureUnit);

	GLuint GetTextureWidth() { return textureWidth; }
	GLuint GetTextureHeight() { return textureHeight; }
	GLuint GetTextureID() { return textureID; }


	~IBLTexture();

protected:
	GLuint FBO, textureID;
	GLuint textureWidth, textureHeight;

};

