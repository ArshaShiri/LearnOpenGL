#ifndef SRC_COMMON_VERTEX_ARRAY
#define SRC_COMMON_VERTEX_ARRAY

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

/**
 * @brief A simple class to wrap a vertex array object
 *
 */
class VertexArray
{
  public:
    /**
     * @brief Constructs a new VertexArray object and store the id
     *
     */
    VertexArray();


    void addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout);

    /**
     * @brief Binds the vertex array object
     *
     */
    void bind() const;

    /**
     * @brief Unbinds the vertex array object
     *
     */
    void unbind() const;

    /**
     * @brief Delete the array
     *
     */
    void deleteArrayObject();

  private:
    unsigned int vertexArrayId_;
};

#endif /* SRC_COMMON_VERTEX_ARRAY */
