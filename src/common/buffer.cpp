#include <stdexcept>

#include <glad/glad.h>

#include "buffer.hpp"

Buffer::Buffer(BufferTye bufferType) : bufferType_{ bufferType }
{
    const auto bufferTypeIsNotDefined =
      (bufferType_ != BufferTye::ArrayBuffer) && (bufferType_ != BufferTye::ElementArrayBuffer);

    if (bufferTypeIsNotDefined)
        throw std::runtime_error("Buffer type is not defined!");

    const auto numberOfBuffers = 1;
    glGenBuffers(numberOfBuffers, &bufferId_);
}

const unsigned int Buffer::getId() const { return bufferId_; }

void Buffer::bind()
{
    switch (bufferType_)
    {
    case BufferTye::ArrayBuffer: {
        glBindBuffer(GL_ARRAY_BUFFER, bufferId_);
        break;
    }
    default:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId_);
        break;
    }
}

void Buffer::unbind()
{
    switch (bufferType_)
    {
    case BufferTye::ArrayBuffer: {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        break;
    }
    default:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        break;
    }
}
