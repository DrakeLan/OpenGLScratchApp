#include "Entity.h"

Entity::Entity()
{
}

void Entity::updateSelfAndChild()
{
    if (parent)
        transform.computeModelMatrix(parent->transform.modelMatrix);
    else
        transform.computeModelMatrix();

    for (auto&& child : children)
    {
        child->updateSelfAndChild();
    }
}

void Entity::AddMaterial(Material* mat)
{
    materialList.push_back(mat);
}

void Entity::RenderEntity()
{
    RemapMeshAndMaterial();
    EntityTraverse(this);
}


void Entity::EntityDraw()
{
    for (size_t i = 0; i < meshList.size(); i++)
    {

        materialList[i]->UseMaterial(this->transform.modelMatrix);
        meshList[i]->RenderMesh();

    }

}

void Entity::EntityTraverse(Entity* theEntity)
{
    theEntity->EntityDraw();
   
    if (theEntity->children.size() > 0)
    {
        Entity* curEntity = theEntity;
        std::list<std::unique_ptr<Entity>>::iterator it;
        for (it = this->children.begin(); it != this->children.end(); ++it) {
            curEntity = it->get();
            EntityTraverse(curEntity);
        }
    }
}

void Entity::RemapMeshAndMaterial()
{
    unsigned int count = 0;
    if (materialList.size() > meshList.size())
    {
        count = materialList.size() - meshList.size();
        for (size_t i = 0; i < count; i++)
        {
            meshList.push_back(meshList[0]);
        }
    }
    else if (materialList.size() < meshList.size())
    {
        count = meshList.size() - materialList.size();
        for (size_t i = 0; i < count; i++)
        {
            materialList.push_back(materialList[0]);
        }

    }

}
