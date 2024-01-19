#include "buffer.hpp"

#include <glad/glad.h>

Buffer::Buffer()
{
    const auto numberOfBuffers = 1;
    glGenBuffers(numberOfBuffers, &bufferId_);
}

const unsigned int Buffer::getId() const { return bufferId_; }

void Buffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, bufferId_); }
void Buffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void Buffer::createAndInitializeBufferData(const std::vector<float> &data)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
}
