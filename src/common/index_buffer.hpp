#ifndef SRC_COMMON_INDEX_BUFFER
#define SRC_COMMON_INDEX_BUFFER

#include <vector>

/**
 * @brief A simple index buffer class
 *
 */
class IndexBuffer
{
  public:
    /**
     * @brief Constructs a new index buffer object and store the id
     *
     */
    IndexBuffer();

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
     * @brief Delete the buffer
     *
     */
    void deleteBuffer();

    std::size_t getCount() const;

    /**
     * @brief Creates a and Initializes buffer data object
     *
     * @param data The data that is going to be copied to the buffer
     */
    void createAndInitializeBufferData(const std::vector<unsigned int> &data);


    ~IndexBuffer();

  private:
    unsigned int bufferId_;
    std::size_t count_;
};

#endif /* SRC_COMMON_INDEX_BUFFER */
