#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return BufferWidth; }
	GLfloat getBufferHeight() { return BufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }



	~Window();

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint BufferWidth, BufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMove;

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int acction, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

};

