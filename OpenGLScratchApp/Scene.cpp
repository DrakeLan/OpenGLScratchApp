#include "Scene.h"

namespace SceneSystem {
	void Scene::AddEntity(Entity* entity)
	{
		sceneEntities.push_back(entity);
	}

	void Scene::RenderScene(Render* render)
	{
		render->getEntities(sceneEntities);
		render->ExecutePasses();
	}
}

