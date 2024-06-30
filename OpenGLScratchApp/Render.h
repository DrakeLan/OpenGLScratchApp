#pragma once
#include "Entity.h"
#include "RenderPass.h"

class Render
{
	public:
		void ExcutePasses();

	
	private:
		list<RenderPass> renderPasses;


};

