#include "Render.h"

namespace RenderSystem {

	Render::Render()
	{
	}

	Render::Render(RenderPass* opaque, RenderPass* transparent)
	{
		opaquePass = opaque;
		transparentPass = transparent;
	}

	void Render::getEntities(vector<Entity*> renderEntities)
	{
		entities = renderEntities;
	}

	void Render::ExecutePasses()
	{
		if ((opaquePass != nullptr) && (transparentPass != nullptr))
		{
			opaquePass->ExecuteRenderPass(entities);

			vector<Mesh*> fMesh = opaquePass->GetFilteredMesh();
			vector<Material*> fMaterial = opaquePass->GetFilteredMaterial();

			if (fMesh.size() > 0)
			{
				transparentPass->ExecuteRenderPass(opaquePass->GetFilteredData());
			}
		}

	}

	Render::~Render()
	{
	}
}