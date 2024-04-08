#include "Entity.h"

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
    Entity* lastEntity = this;
    while (lastEntity->children.size())
    {
        lastEntity->material.UseMaterial(lastEntity->transform.modelMatrix);
        lastEntity->RenderModel();;
    }
}
