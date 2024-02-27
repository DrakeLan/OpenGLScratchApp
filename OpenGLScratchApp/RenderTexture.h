#pragma once

#include <stdio.h>
#include <GL\glew.h>

class RenderTexture
{
	public:
		RenderTexture();

		bool Init_SRGBA(GLuint width, GLuint height);

		void BindFBO();
		void Clear();
		void Write();
		void Read(GLenum textureUnit);

		GLuint GetTexureID();

		~RenderTexture();


	private:
		GLuint FBO, textureID, depthBuffer;
		GLuint textureWidth, textureHeight;

};

