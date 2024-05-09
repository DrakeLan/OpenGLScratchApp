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

void Entity::RenderEntity()
{
    EntityTraverse(this);
}


void Entity::EntityDraw()
{
    if (materialList.size() > meshList.size())
    {
        for (size_t i = 0; i < materialList.size(); i++)
        {
            if (i < meshList.size())
            {
                materialList[i]->UseMaterial(this->transform.modelMatrix);
                meshList[i]->RenderMesh();
            }
        }
    }
    else
    {
        for (size_t i = 0; i < meshList.size(); i++)
        {
            if (i < materialList.size())
            {
                materialList[i]->UseMaterial(this->transform.modelMatrix);
                meshList[i]->RenderMesh();
            }
            else
            {
                materialList[0]->UseMaterial(this->transform.modelMatrix);
                meshList[i]->RenderMesh();
            }
        }
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
