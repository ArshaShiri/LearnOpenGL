#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <stb_image/stb_image.h>

#include "renderer.hpp"
#include "texture.hpp"

Texture::Texture(std::string path, bool flipVertically) : textureFilePath_{ std::move(path) }
{
    if (flipVertically)
        // tell stb_image.h to flip loaded texture's on the y-axis.
        stbi_set_flip_vertically_on_load(true);

    try
    {
        initialize();
    }
    catch (const std::exception &e)
    {
        std::cout << "Texture exception with message: '" << e.what() << "'\n";
    }
}

void Texture::bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, textureId_));
}

void Texture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }

Texture::~Texture() { GLCall(glDeleteTextures(1, &textureId_)); }

void Texture::initialize()
{
    GLCall(glGenTextures(1, &textureId_));
    GLCall(glBindTexture(GL_TEXTURE_2D, textureId_));

    const auto textureFilePath = std::filesystem::current_path().append("src/resources/textures/container.jpg");

    static constexpr std::size_t numberOfDesiredChannels = 4;
    localBuffer_ = stbi_load(textureFilePath.c_str(), &width_, &height_, &bitsPerPixel_, numberOfDesiredChannels);

    if (!localBuffer_)
        throw std::runtime_error("Shader type is not supported!");

    // set texture filtering parameters
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // set the texture wrapping parameters
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer_));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(localBuffer_);
    unbind();
}
