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
    EntityTraverse(this);
}

void Entity::EntityDraw()
{
    this->material.UseMaterial(this->transform.modelMatrix);
    this->RenderModel();
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
