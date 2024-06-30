#pragma once
#include "Render.h"


class Scene
{
	public:
		void AddEntity(Entity* entity);
		void RenderScene(Render* render);


	private:
		list<Entity*> sceneEntities;

};

