#include <stdexcept>

#include <glad/glad.h>

#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer()
{
    const auto numberOfBuffers = 1;
    GLCall(glGenBuffers(numberOfBuffers, &bufferId_));
}

unsigned int VertexBuffer::getId() const { return bufferId_; }

void VertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferId_)); }

void VertexBuffer::unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void VertexBuffer::deleteBuffer() { GLCall(glDeleteBuffers(1, &bufferId_)); }

// Maybe delete the buffer here if it is not deleted already
VertexBuffer::~VertexBuffer() {}
