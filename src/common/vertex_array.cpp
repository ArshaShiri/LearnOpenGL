#include "vertex_array.hpp"

#include <glad/glad.h>

VertexArray::VertexArray()
{
    const auto numberOfVertexArrays = 1;
    glGenVertexArrays(numberOfVertexArrays, &vertexArrayId_);
}

void VertexArray::bind() { glBindVertexArray(vertexArrayId_); }
