#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Transform
{
	public:
		Transform();

		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		glm::mat4 modelMatrix = glm::mat4(1.0f);

	private:
		bool dirtyFlag = true;

	protected:
		glm::mat4 getLocalModelMatrix();

	public:
		void computeModelMatrix();
		void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix);

};

