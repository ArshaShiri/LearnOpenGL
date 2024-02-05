
#include <iostream>

#include <glad/glad.h>

#include "renderer.hpp"
#include "vertex_array.hpp"

VertexArray::VertexArray()
{
    const auto numberOfVertexArrays = 1;
    GLCall(glGenVertexArrays(numberOfVertexArrays, &vertexArrayId_));
}

void VertexArray::addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout)
{
    bind();
    vertexBuffer.bind();

    const auto &elements = layout.getElements();
    const auto stride = static_cast<int>(layout.getStride());
    unsigned int offset = 0;

    for (unsigned int elementIndex = 0; elementIndex < elements.size(); ++elementIndex)
    {
        const auto &element = elements[elementIndex];
        GLCall(glEnableVertexAttribArray(elementIndex));
        GLCall(glVertexAttribPointer(elementIndex,
                                     static_cast<int>(element.count),
                                     element.type,
                                     element.normalized,
                                     stride,
                                     reinterpret_cast<void *>(offset)));

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const { GLCall(glBindVertexArray(vertexArrayId_)); }

void VertexArray::unbind() const { GLCall(glBindVertexArray(0)); }

void VertexArray::deleteArrayObject() { GLCall(glDeleteVertexArrays(1, &vertexArrayId_)); }
