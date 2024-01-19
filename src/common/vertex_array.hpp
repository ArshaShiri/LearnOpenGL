#ifndef SRC_COMMON_VERTEX_ARRAY
#define SRC_COMMON_VERTEX_ARRAY

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

    /**
     * @brief Binds the vertex array object
     *
     */
    void bind();

  private:
    unsigned int vertexArrayId_;
};

#endif /* SRC_COMMON_VERTEX_ARRAY */
