#pragma once
#include "Model.h"
#include "Transform.h"
#include "Material.h"
#include <list>
#include <memory>

class Entity :
    public Model
{
    public:

		Transform transform;
		Material material;

		Entity() : Model() {}

		std::list<std::unique_ptr<Entity>> children;
		Entity* parent = nullptr;

		template<typename... TArgs>
		void addChild(const TArgs... args);

		void updateSelfAndChild();

		void RenderEntity();

};

template<typename ...TArgs>
inline void Entity::addChild(const TArgs ...args)
{
	children.emplace_back(std::make_unique<Entity>(args...));
	children.back()->parent = this;
}
