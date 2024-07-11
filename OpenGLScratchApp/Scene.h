#pragma once
#include "Render.h"

using namespace RenderSystem;

namespace SceneSystem {
	class Scene
	{
	public:
		void AddEntity(Entity* entity);
		void RenderScene(RenderSystem::Render* render);


	private:
		vector<Entity*> sceneEntities;

	};
}


