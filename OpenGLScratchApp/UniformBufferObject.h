#pragma once

#include <stdio.h>
#include <iostream>

#include <GL\glew.h>
#include <GLM\glm.hpp>

class UniformBufferObject
{

public:
    ~UniformBufferObject();
    
    UniformBufferObject();
    UniformBufferObject(const size_t byteSize, GLenum usageHint = GL_STREAM_DRAW);
    void createUBO(const size_t byteSize, GLenum usageHint);
    void bindUBO() const;
    void setBufferData(const size_t offset, const void* ptrData, const size_t dataSize);
    void bindBufferBaseToBindingPoint(const GLuint bindingPoint);
    GLuint getBufferID() const;
    void deleteUBO();
    

private:
    GLuint _bufferID{ 0 };
    size_t _byteSize;
    
    //bool _isBufferCreated = false;

};

