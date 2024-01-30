#ifndef SRC_COMMON_VERTEX_BUFFER_LAYOUT
#define SRC_COMMON_VERTEX_BUFFER_LAYOUT

#include <stdexcept>
#include <vector>

#include <glad/glad.h>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            throw std::runtime_error("Unsupported type");
        }
    }
};

class VertexBufferLayout
{
  public:
    template<typename T>
    void push(unsigned int count);

    const std::vector<VertexBufferElement> &getElements() const { return elements_; }
    unsigned int getStride() const { return stride_; }

  private:
    unsigned int stride_ = 0;
    std::vector<VertexBufferElement> elements_;
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
    elements_.emplace_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
    stride_ += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    elements_.emplace_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
    stride_ += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    elements_.emplace_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
    stride_ += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
}

#endif /* SRC_COMMON_VERTEX_BUFFER_LAYOUT */
