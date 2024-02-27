#include "RenderTexture.h"

RenderTexture::RenderTexture()
{
    FBO = 0;
    textureID = 0;
	depthBuffer = 0;
    textureHeight = 0.0f;
    textureWidth = 0.0f;
}

bool RenderTexture::Init_SRGBA(GLuint width, GLuint height)
{
	textureWidth = width;
	textureHeight = height;

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, (GLuint)depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void RenderTexture::BindFBO()
{
	glViewport(0, 0, textureWidth, textureHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void RenderTexture::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::Write()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
}

void RenderTexture::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint RenderTexture::GetTexureID()
{
	return textureID;
}

RenderTexture::~RenderTexture()
{
	glDeleteFramebuffers(1, &FBO);
}
