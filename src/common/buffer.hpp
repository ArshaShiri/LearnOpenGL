#ifndef SRC_COMMON_BUFFER
#define SRC_COMMON_BUFFER

#include <vector>

/**
 * @brief A simple buffer class that only supports binging to GL_ARRAY_BUFFER
 *
 */
class Buffer
{
  public:
    /**
     * @brief Constructs a new Buffer object and store the id
     *
     */
    Buffer();

    /**
     * @brief Get the Id of the buffer
     *
     * @return const unsigned int
     */
    const unsigned int getId() const;

    /**
     * @brief Binds to target //!(Only supports GL_ARRAY_BUFFER)
     *
     */
    void bindBuffer();

    /**
     * @brief Creates a and Initializes buffer data object
     * !(Only supports GL_ARRAY_BUFFER and GL_STATIC_DRAW)
     *
     * @param data The data that is going to be copied to the buffer
     */
    void createAndInitializeBufferData(const std::vector<float> &data);

  private:
    unsigned int bufferId_;
};

#endif /* SRC_COMMON_BUFFER */
