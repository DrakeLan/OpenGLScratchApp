#pragma once
#include "RenderPass.h"

namespace RenderSystem {

	class Render
	{
	public:
		Render();
		Render(RenderPass* opaque, RenderPass* transparent);
		void getEntities(vector<Entity*> renderEntities);
		void ExecutePasses();


		~Render();

	private:
		list<RenderPass> renderPasses;
		RenderPass* opaquePass;
		RenderPass* transparentPass;
		//Keep entities here, in case do occlusion culling in future
		vector<Entity*> entities;


	};

}


