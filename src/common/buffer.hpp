#ifndef SRC_COMMON_BUFFER
#define SRC_COMMON_BUFFER

#include <vector>

enum class BufferTye
{
    ArrayBuffer,
    ElementArrayBuffer
};

/**
 * @brief A simple buffer class that only supports binging to GL_ARRAY_BUFFER
 * !(Only supports GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER)
 *
 */
class Buffer
{
  public:
    /**
     * @brief Constructs a new Buffer object and store the id
     *
     */
    Buffer(BufferTye bufferType);

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
    void bind();

    /**
     * @brief Unbind buffer
     *
     */
    void unbind();

    /**
     * @brief Creates a and Initializes buffer data object
     *
     * @param data The data that is going to be copied to the buffer
     */
    template<typename T>
    void createAndInitializeBufferData(const std::vector<T> &data)
    {
        switch (bufferType_)
        {
        case BufferTye::ArrayBuffer: {
            glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);
            break;
        }
        default:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);
            break;
        }
    }

  private:
    unsigned int bufferId_;
    BufferTye bufferType_;
};

#endif /* SRC_COMMON_BUFFER */
