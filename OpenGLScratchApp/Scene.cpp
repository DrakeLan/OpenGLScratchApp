#include "Scene.h"

void Scene::AddEntity(Entity* entity)
{
	sceneEntities.push_back(entity);
}

void Scene::RenderScene(Render* render)
{
	render->ExecutePasses();
}

