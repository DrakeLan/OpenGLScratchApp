#pragma once
#include "Entity.h"

namespace RenderSystem {

	enum RenderOrder
	{
		FrontToBack,
		BackToFront
	};

	struct FilteredRenderData
	{
		RenderOrder sortOrder;
		vector<glm::mat4> modelTransMat;
		vector<Mesh*> filteredMesh;
		vector<Material*> filteredMaterial;
	};

	class RenderPass
	{
	public:
		RenderPass();
		RenderPass(RenderMode rMode);

		void ExecuteRenderPass(vector<Entity*> rootEntities);
		void ExecuteRenderPass(FilteredRenderData filteredRenderData);

		vector<Mesh*> GetFilteredMesh();
		vector<Material*> GetFilteredMaterial();
		FilteredRenderData GetFilteredData();

		~RenderPass();

	private:
		RenderMode renderMode;
		FilteredRenderData filteredData;

		void DrawEntity(Entity* theEntity, RenderMode rednerMode);
		void TraverseEntity(Entity* theEntity, RenderMode renderMode);

		void DrawMeshes(FilteredRenderData filteredRenderData, RenderMode renderMode);

	};
}


