// Try drawing a second container with another call to glDrawElements but place it at a different position using
// transformations only. Make sure this second container is placed at the top-left of the window and instead of
// rotating, scale it over time (using the sin function is useful here; note that using sin will cause the object to
// invert as soon as a negative scale is applied)

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
// Do not change the order of include files here...
#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/index_buffer.hpp"
#include "common/program.hpp"
#include "common/renderer.hpp"
#include "common/shader.hpp"
#include "common/vertex_array.hpp"
#include "common/vertex_buffer.hpp"
#include "common/vertex_buffer_layout.hpp"
#include "common/window.hpp"


int main()
{
    const auto windowWidth = 800;
    const auto windowHeight = 600;

    auto window = Window(windowWidth, windowHeight);

    // TODO Having the path like this is quite fragile since it depends on the path from which the binary is run.
    // TODO It is much better if the shader paths are passed as command line arguments.
    const auto vertexShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/5_1_transformations/vertex_shader.vs");
    const auto vertexShader = Shader(ShaderType::Vertex, "", vertexShaderFilePath);

    const auto fragmentShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/5_1_transformations/fragment_shader.fs");
    const auto fragmentShader = Shader(ShaderType::Fragment, "", fragmentShaderFilePath);

    auto shaderProgram = Program();
    shaderProgram.attachShader(vertexShader.getShaderId());
    shaderProgram.attachShader(fragmentShader.getShaderId());
    shaderProgram.linkProgram();
    vertexShader.deleteShader();
    fragmentShader.deleteShader();

    // clang-format off
    std::vector<float> vertices{
        // positions         // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
    };

     std::vector<unsigned int> indices{
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // clang-format on

    auto vertexBufferObject = VertexBuffer{ vertices };
    auto vertexArrayObject = VertexArray{};

    auto vertexBufferLayout = VertexBufferLayout{};
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(2);

    vertexArrayObject.addBuffer(vertexBufferObject, vertexBufferLayout);

    auto indexBuffer = IndexBuffer{};
    indexBuffer.bind();
    indexBuffer.createAndInitializeBufferData(indices);
    vertexBufferObject.unbind();


    // load and create a texture
    // -------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D,
                  texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    const auto texture1FilePath = std::filesystem::current_path().append("src/resources/textures/container.jpg");
    unsigned char *data = stbi_load(texture1FilePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // texture 2
    // ---------
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    const auto texture2FilePath = std::filesystem::current_path().append("src/resources/textures/awesomeface.png");
    data = stbi_load(texture2FilePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the
        //! data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shaderProgram.useProgram(); // don't forget to activate/use the shader before setting uniforms!
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    // translation
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    shaderProgram.setMatrix4fv("transform", trans);

    // unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    Renderer renderer;

    // render loop
    while (!window.shouldClose())
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_2D, texture2));

        // Spinning logic
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
        shaderProgram.setMatrix4fv("transform", trans);

        renderer.draw(vertexArrayObject, indexBuffer, shaderProgram);

        // second transformation
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        shaderProgram.setMatrix4fv("transform", trans);

        renderer.draw(vertexArrayObject, indexBuffer, shaderProgram);

        window.swapBuffers();
        glfwPollEvents();
    }

    vertexArrayObject.deleteArrayObject();
    vertexBufferObject.deleteBuffer();
    shaderProgram.deleteProgram();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
