#pragma once
#include "Model.h"
#include "Transform.h"
#include "Material.h"
#include <list>
#include <memory>

class Entity
{
    public:

		Transform transform;
		std::vector<Material*> materialList;
		std::vector<Mesh*> meshList;

		Entity();

		std::list<std::unique_ptr<Entity>> children;
		Entity* parent = nullptr;

		template<typename... TArgs>
		void addChild(const TArgs... args);

		void updateSelfAndChild();

		void AddMaterial(Material* mat);
		void RemapMeshAndMaterial();
		void RenderEntity();

	private:
		void EntityDraw();
		void EntityTraverse(Entity* theEntity);
		

};

template<typename ...TArgs>
inline void Entity::addChild(const TArgs ...args)
{
	children.emplace_back(std::make_unique<Entity>(args...));
	children.back()->parent = this;
}
