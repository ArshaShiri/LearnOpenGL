#include "vertex_array.hpp"

#include <glad/glad.h>

VertexArray::VertexArray()
{
    const auto numberOfVertexArrays = 1;
    glGenVertexArrays(numberOfVertexArrays, &vertexArrayId_);
}

void VertexArray::addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout)
{
    bind();
    vertexBuffer.bind();
    const auto &elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int elementIndex = 0; elementIndex < elements.size(); ++elementIndex)
    {
        const auto &element = elements[elementIndex];
        glEnableVertexAttribArray(elementIndex);
        glVertexAttribPointer(
          elementIndex, element.count, element.type, element.normalized, layout.getStride(), (void *)(offset));

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const { glBindVertexArray(vertexArrayId_); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::deleteArrayObject() { glDeleteVertexArrays(1, &vertexArrayId_); }
