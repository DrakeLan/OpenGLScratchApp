#pragma once
#include "Entity.h"

enum RenderOrder
{
	FrontToBack,
	BackToFront
};

struct FilteredRenderData 
{
	RenderOrder sortOrder;
	vector<Mesh*> filteredMesh;
	vector<Material*> filteredMaterial;
};

class RenderPass
{
	public:
		void ExecuteRenderPass(list<Entity*> Entities);
		void ExecuteRenderPass(list<Entity*> Entities, RenderMode renderMode);
		void ExecuteRenderPass(FilteredRenderData filteredRenderData, RenderMode renderMode);

	private:
		FilteredRenderData filteredData;

		void DrawEntity(Entity* theEntity, RenderMode rednerMode);
		void TraverseEntity(Entity* theEntity, RenderMode renderMode);

		void DrawMeshes(FilteredRenderData filteredRenderData, RenderMode renderMode);
		 
};



