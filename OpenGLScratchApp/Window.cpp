#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) 
{
	width = windowWidth;
	height = windowHeight;

	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;

	}

	//Set up GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Create Window
	mainWindow = glfwCreateWindow(width, height, "Test window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}
	
	//Set buffer size information
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle Key + Mouse Input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Alow modern extension featrues
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//Setup Viewport size
	glViewport(0, 0, BufferWidth, BufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}



void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			thisWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			thisWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (thisWindow->mouseFirstMove)
	{
		thisWindow->lastX = xPos;
		thisWindow->lastY = yPos;
		thisWindow->mouseFirstMove = false;
	}

	thisWindow->xChange = xPos - thisWindow->lastX;
	thisWindow->yChange = thisWindow->lastY - yPos;


	thisWindow->lastX = xPos;
	thisWindow->lastY = yPos;

	//printf("x:%.6f, y:%.6f\n", thisWindow->xChange, thisWindow->yChange);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}