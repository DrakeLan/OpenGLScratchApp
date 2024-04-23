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
    /*Entity* curEntity = this;
    std::list<std::unique_ptr<Entity>>::iterator it;
    for (it = this->children.begin(); it != this->children.end(); ++it) {
        curEntity = it->get();
        if (curEntity->children.size() > 0)
        {

        }
    }


    Entity* lastEntity = this;
    while (lastEntity->children.size())
    {
        lastEntity->material.UseMaterial(lastEntity->transform.modelMatrix);
        lastEntity->RenderModel();
        lastEntity = lastEntity->children.back().get();
    }*/
    this->material.UseMaterial(this->transform.modelMatrix);
    this->RenderModel();
}
