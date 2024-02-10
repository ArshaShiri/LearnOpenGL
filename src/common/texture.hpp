#ifndef SRC_COMMON_TEXTURE
#define SRC_COMMON_TEXTURE

#include <string>

/**
 * @brief A simple texture class for a png file
 *
 */
class Texture
{
  public:
    Texture(std::string path, bool flipVertically = false);

    /**
     * @brief Bind texture
     *
     * @param slot optional parameter to specify the slot that we want to bind the texture to.
     */
    void bind(unsigned int slot = 0) const;

    /**
     * @brief Unbind texture
     *
     */
    void unbind() const;

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    ~Texture();

  private:
    void initialize();

    unsigned int textureId_ = 0;
    int width_ = 0;
    int height_ = 0;
    int bitsPerPixel_ = 0;
    std::string textureFilePath_;
    unsigned char *localBuffer_ = nullptr;
};

#endif /* SRC_COMMON_TEXTURE */
