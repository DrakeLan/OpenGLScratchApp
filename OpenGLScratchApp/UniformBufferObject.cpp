#include "UniformBufferObject.h"



UniformBufferObject::UniformBufferObject()
{
    _bufferID = 0;
    _byteSize = 0;
}

void UniformBufferObject::createUBO(const size_t byteSize, GLenum usageHint)
{

    
    glGenBuffers(1, &_bufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
    glBufferData(GL_UNIFORM_BUFFER, byteSize, NULL, usageHint);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    _byteSize = byteSize;
}

void UniformBufferObject::bindUBO() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
}



void UniformBufferObject::setBufferData(const size_t offset, const void* ptrData, const size_t dataSize)
{
    if (offset >= _byteSize)
    {
        std::cerr << "Tried to set data of uniform buffer object at offset " << offset << ", but it's beyond buffer size " << _byteSize << ", will be ignored..." << std::endl;
        return;
    }

    if (offset + dataSize > _byteSize)
    {
        std::cerr << "Could not set data because it would overflow the buffer! Offset: " << offset << ", data size: " << dataSize << std::endl;
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, ptrData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::bindBufferBaseToBindingPoint(const GLuint bindingPoint)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _bufferID);
}

GLuint UniformBufferObject::getBufferID() const
{
	return _bufferID;
}

void UniformBufferObject::deleteUBO()
{
    glDeleteBuffers(1, &_bufferID);
}


UniformBufferObject::~UniformBufferObject()
{
	deleteUBO();
}

