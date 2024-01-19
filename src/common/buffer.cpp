#include "buffer.hpp"

#include <glad/glad.h>

Buffer::Buffer()
{
    const auto numberOfBuffers = 1;
    glGenBuffers(numberOfBuffers, &bufferId_);
}

const unsigned int Buffer::getId() const { return bufferId_; }

void Buffer::bindBuffer() { glBindBuffer(GL_ARRAY_BUFFER, bufferId_); }
