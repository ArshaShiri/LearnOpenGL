
#include <iostream>

#include <glad/glad.h>

#include "renderer.hpp"
#include "vertex_array.hpp"

VertexArray::VertexArray()
{
    const auto numberOfVertexArrays = 1;
    glGenVertexArrays(numberOfVertexArrays, &vertexArrayId_);
}

void VertexArray::addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout)
{
    auto checkErrors = []() {
        while (GLenum error = glGetError())
        {
            std::cout << error << std::endl;
        }
    };

    bind();
    vertexBuffer.bind();
    const auto &elements = layout.getElements();
    unsigned int offset = 0;
    checkErrors();

    for (unsigned int elementIndex = 0; elementIndex < elements.size(); ++elementIndex)
    {
        const auto &element = elements[elementIndex];
        GLCall(glEnableVertexAttribArray(elementIndex));
        GLCall(glVertexAttribPointer(elementIndex,
                                     element.count,
                                     element.type,
                                     element.normalized,
                                     layout.getStride(),
                                     reinterpret_cast<void *>(offset)));

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const { GLCall(glBindVertexArray(vertexArrayId_)); }

void VertexArray::unbind() const { GLCall(glBindVertexArray(0)); }

void VertexArray::deleteArrayObject() { GLCall(glDeleteVertexArrays(1, &vertexArrayId_)); }
