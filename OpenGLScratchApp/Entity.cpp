#include "Entity.h"

glm::mat4 Entity::getLocalModelMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
        glm::radians(transform.eulerRot.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
        glm::radians(transform.eulerRot.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
        glm::radians(transform.eulerRot.z),
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 roationMatrix = transformY * transformX * transformZ;

    return glm::translate(glm::mat4(1.0f), transform.pos) *
        roationMatrix *
        glm::scale(glm::mat4(1.0f), transform.scale);
}

void Entity::updateSelfAndChild()
{
    if (parent)
        transform.modelMatrix = parent->transform.modelMatrix * getLocalModelMatrix();
    else
        transform.modelMatrix = getLocalModelMatrix();

    for (auto&& child : children)
    {
        child->updateSelfAndChild();
    }
}
