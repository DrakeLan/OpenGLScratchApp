#pragma once
#include "Model.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <list>
#include <memory>

struct Transform
{
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 modelMatrix = glm::mat4(1.0f);

};

class Entity :
    public Model
{
    public:

		Transform transform;

		Entity() : Model() {}

		std::list<std::unique_ptr<Entity>> children;
		Entity* parent = nullptr;

		template<typename... TArgs>
		void addChild(const TArgs... args);

		glm::mat4 getLocalModelMatrix();
		void updateSelfAndChild();



};

template<typename ...TArgs>
inline void Entity::addChild(const TArgs ...args)
{
	children.emplace_back(std::make_unique<Entity>(args...));
	children.back()->parent = this;
}
