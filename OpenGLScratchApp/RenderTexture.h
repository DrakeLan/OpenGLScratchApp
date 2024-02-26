#pragma once

#include <stdio.h>
#include <GL\glew.h>

class RenderTexture
{
	public:
		RenderTexture();

		bool Init();

		void BindFBO();
		void Write();
		void Read();

		~RenderTexture();


	private:
		GLuint FBO, textureID;
		GLuint textureWidth, textureHeight;

};

