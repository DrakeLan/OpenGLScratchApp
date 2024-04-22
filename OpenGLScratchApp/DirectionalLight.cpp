#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(): Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, 
									GLfloat red, GLfloat green, GLfloat blue,
									GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir):Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	direction.x = xDir;
	direction.y = yDir;
	direction.z = zDir;

	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 200.0f);
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
	GLfloat diffuseIntensityLocation, GLfloat dirctionLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(dirctionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::vec3 DirectionalLight::GetDirection()
{
	return direction;
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{

}