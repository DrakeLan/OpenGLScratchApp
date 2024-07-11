#include "RenderPass.h"
namespace RenderSystem {

    RenderPass::RenderPass()
    {
        renderMode = opaque;
    }

    RenderPass::RenderPass(RenderMode rMode)
    {
        renderMode = rMode;
    }

    void RenderPass::ExecuteRenderPass(vector<Entity*> rootEntities)
    {
        for (size_t i = 0; i < rootEntities.size(); i++)
        {
            TraverseEntity(rootEntities[i], renderMode);
        }

    }

    void RenderPass::ExecuteRenderPass(FilteredRenderData filteredRenderData)
    {
    }

    vector<Mesh*> RenderPass::GetFilteredMesh()
    {
        return filteredData.filteredMesh;
    }

    vector<Material*> RenderPass::GetFilteredMaterial()
    {
        return filteredData.filteredMaterial;
    }

    FilteredRenderData RenderPass::GetFilteredData()
    {
        return filteredData;
    }

    RenderPass::~RenderPass()
    {
    }

    void RenderPass::DrawEntity(Entity* theEntity, RenderMode renderMode)
    {
        Mesh* curMesh;
        Material* curMat;

        theEntity->RemapMeshAndMaterial();

        if (renderMode == opaque)
        {
            filteredData.sortOrder = FrontToBack;
            for (size_t i = 0; i < theEntity->materialList.size(); i++)
            {
                curMat = theEntity->materialList[i];
                curMesh = theEntity->meshList[i];

                if (curMat->renderMode == renderMode)
                {
                    curMat->UseMaterial(theEntity->transform.modelMatrix);
                    curMesh->RenderMesh();
                }
                else
                {
                    filteredData.filteredMesh.push_back(curMesh);
                    filteredData.filteredMaterial.push_back(curMat);
                }
            }
        }
        else
        {
            filteredData.sortOrder = BackToFront;
            for (size_t i = theEntity->materialList.size() - 1; i >= 0; i--)
            {
                curMat = theEntity->materialList[i];
                curMesh = theEntity->meshList[i];

                if (curMat->renderMode == renderMode)
                {
                    curMat->UseMaterial(theEntity->transform.modelMatrix);
                    curMesh->RenderMesh();
                }
                else
                {
                    filteredData.filteredMesh.push_back(curMesh);
                    filteredData.filteredMaterial.push_back(curMat);
                }
            }
        }


    }

    void RenderPass::TraverseEntity(Entity* theEntity, RenderMode renderMode)
    {
        DrawEntity(theEntity, renderMode);

        if (theEntity->children.size() > 0)
        {
            Entity* curEntity = theEntity;
            std::list<std::unique_ptr<Entity>>::iterator it;
            for (it = theEntity->children.begin(); it != theEntity->children.end(); ++it) {
                curEntity = it->get();
                TraverseEntity(curEntity, renderMode);
            }
        }
    }

    void RenderPass::DrawMeshes(FilteredRenderData filteredRenderData, RenderMode renderMode)
    {
        if (renderMode == filteredRenderData.sortOrder)
        {
            for (size_t i = 0; i < filteredRenderData.filteredMaterial.size(); i++)
            {
                filteredRenderData.filteredMaterial[i]->UseMaterial();
                filteredRenderData.filteredMesh[i]->RenderMesh();
            }
        }
        else
        {
            for (size_t i = filteredRenderData.filteredMaterial.size(); i >= 0; i--)
            {
                filteredRenderData.filteredMaterial[i]->UseMaterial();
                filteredRenderData.filteredMesh[i]->RenderMesh();
            }
        }
    }
}


