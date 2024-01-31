#ifndef SRC_COMMON_VERTEX_BUFFER
#define SRC_COMMON_VERTEX_BUFFER

#include <vector>

#include <glad/glad.h>

#include "renderer.hpp"


/**
 * @brief A simple vertex buffer class
 *
 */
class VertexBuffer
{
  public:
    /**
     * @brief Constructs a new vertex buffer object and store the id
     *
     */
    VertexBuffer();

    template<typename T>
    VertexBuffer(const std::vector<T> &data) : VertexBuffer()
    {
        bind();
        createAndInitializeBufferData(data);
    }

    /**
     * @brief Get the Id of the buffer
     *
     * @return const unsigned int
     */
    const unsigned int getId() const;

    /**
     * @brief Binds to target
     *
     */
    void bind() const;

    /**
     * @brief Unbind buffer
     *
     */
    void unbind() const;

    /**
     * @brief Delete the buffer
     *
     */
    void deleteBuffer();

    /**
     * @brief Creates a and Initializes buffer data object
     *
     * @param data The data that is going to be copied to the buffer
     */
    template<typename T>
    void createAndInitializeBufferData(const std::vector<T> &data)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW));
    }

    ~VertexBuffer();

  private:
    unsigned int bufferId_;
};

#endif /* SRC_COMMON_VERTEX_BUFFER */
