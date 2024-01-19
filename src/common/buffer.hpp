#ifndef SRC_COMMON_BUFFER
#define SRC_COMMON_BUFFER

/**
 * @brief A simple buffer class that only supports binging to GL_ARRAY_BUFFER
 *
 */
class Buffer
{
  public:
    /**
     * @brief Construct a new Buffer object and store the id
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
     * @brief Bind to target (Only supports GL_ARRAY_BUFFER)
     *
     */
    void bindBuffer();

  private:
    unsigned int bufferId_;
};

#endif /* SRC_COMMON_BUFFER */
