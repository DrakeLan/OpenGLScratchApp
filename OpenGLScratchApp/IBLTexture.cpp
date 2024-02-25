#include "IBLTexture.h"

IBLTexture::IBLTexture()
{
    FBO = 0;
    textureID = 0;
    textureHeight = 0.0f;
    textureWidth = 0.0f;
}

bool IBLTexture::InitCube(GLuint width, GLuint height, bool mipFlag)
{
	textureHeight = height;
	textureWidth = width;

	glGenFramebuffers(1, &FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, FBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBO);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	if (mipFlag)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	else 
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;

}

bool IBLTexture::InitLUT(GLuint width, GLuint height)
{
	textureHeight = height;
	textureWidth = width;

	glGenFramebuffers(1, &FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, FBO);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, textureHeight, textureWidth, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void IBLTexture::BindFBO()
{
	glViewport(0, 0, textureWidth, textureHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	
}

void IBLTexture::Write()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
}

void IBLTexture::Write(GLuint i, GLuint mip)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureID, mip);
}

void IBLTexture::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

IBLTexture::~IBLTexture()
{
}
