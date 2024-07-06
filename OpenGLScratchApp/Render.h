#pragma once
#include "RenderPass.h"

class Render
{
	public:
		void ExecutePasses();

	
	private:
		list<RenderPass> renderPasses;
		//Keep entities here, in case do occlusion culling in future
		list<Entity*> entities;


};

