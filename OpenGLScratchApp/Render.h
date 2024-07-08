#pragma once
#include "RenderPass.h"

class Render
{
	public:
		Render(RenderPass* opaque, RenderPass* transparent);
		void getEntities(vector<Entity*> renderEntities);
		void ExecutePasses();

	
	private:
		list<RenderPass> renderPasses;
		RenderPass* opaquePass;
		RenderPass* transparentPass;
		//Keep entities here, in case do occlusion culling in future
		vector<Entity*> entities;


};

