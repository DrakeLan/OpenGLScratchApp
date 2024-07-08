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
		void ExecuteRenderPass(vector<Entity*> Entities);
		void ExecuteRenderPass(vector<Entity*> Entities, RenderMode renderMode);
		void ExecuteRenderPass(FilteredRenderData filteredRenderData, RenderMode renderMode);

		vector<Mesh*> GetFilteredMesh();
		vector<Material*> GetFilteredMaterial();
		FilteredRenderData GetFilteredData();

	private:
		RenderMode renderMode;
		FilteredRenderData filteredData;

		void DrawEntity(Entity* theEntity, RenderMode rednerMode);
		void TraverseEntity(Entity* theEntity, RenderMode renderMode);

		void DrawMeshes(FilteredRenderData filteredRenderData, RenderMode renderMode);
		 
};



