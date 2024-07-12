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
			//glEnable(GL_DEPTH_TEST);
			opaquePass->ExecuteRenderPass(entities);

			vector<Mesh*> fMesh = opaquePass->GetFilteredMesh();
			vector<Material*> fMaterial = opaquePass->GetFilteredMaterial();

			if (fMesh.size() > 0)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glDepthMask(GL_FALSE);
				transparentPass->ExecuteRenderPass(opaquePass->GetFilteredData());
				glDisable(GL_BLEND);
				//glDepthMask(GL_TRUE);
			}
		}

	}

	Render::~Render()
	{
	}
}