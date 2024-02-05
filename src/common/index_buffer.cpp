#include <stdexcept>

#include <glad/glad.h>

#include "index_buffer.hpp"
#include "renderer.hpp"

IndexBuffer::IndexBuffer()
{
    const auto numberOfBuffers = 1;
    glGenBuffers(numberOfBuffers, &bufferId_);
}

unsigned int IndexBuffer::getId() const { return bufferId_; }

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId_)); }

void IndexBuffer::unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

void IndexBuffer::deleteBuffer() { GLCall(glDeleteBuffers(1, &bufferId_)); }

void IndexBuffer::createAndInitializeBufferData(const std::vector<unsigned int> &data)
{
    count_ = data.size();
    GLCall(glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, static_cast<long int>(sizeof(unsigned int) * count_), data.data(), GL_STATIC_DRAW));
}

std::size_t IndexBuffer::getCount() const { return count_; }

// Maybe delete the buffer here if it is not deleted already
IndexBuffer::~IndexBuffer() {}
