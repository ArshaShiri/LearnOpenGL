#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
// Do not change the order of include files here...
#include <GLFW/glfw3.h>

#include "common/program.hpp"
#include "common/shader.hpp"
#include "common/vertex_array.hpp"
#include "common/vertex_buffer.hpp"
#include "common/window.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void checkErrors()
{
    while (GLenum error = glGetError())
    {
        std::cout << error << std::endl;
    }
}

int main()
{
    const auto windowWidth = 800;
    const auto windowHeight = 600;

    // TODO: This call silently initializes glad stuff which is not related to window.
    auto window = Window(windowWidth, windowHeight);

    // TODO Having the path like this is quite fragile since it depends on the path from which the binary is run.
    // TODO It is much better if the shader paths are passed as command line arguments.
    const auto vertexShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/3_2_shaders_interpolation/vertex_shader.fs");
    const auto vertexShader = Shader(ShaderType::Vertex, "", vertexShaderFilePath);

    const auto fragmentShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/3_2_shaders_interpolation/fragment_shader.fs");
    const auto fragmentShader = Shader(ShaderType::Fragment, "", fragmentShaderFilePath);

    auto shaderProgram = Program();
    shaderProgram.attachShader(vertexShader.getShaderId());
    shaderProgram.attachShader(fragmentShader.getShaderId());

    shaderProgram.linkProgram();

    vertexShader.deleteShader();
    fragmentShader.deleteShader();


    std::vector<float> vertices{
        // positions        // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f // top
    };

    auto vertexBufferObject = VertexBuffer{};
    auto vertexArrayObject = VertexArray{};

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    vertexArrayObject.bind();

    // 0. copy our vertices array in a buffer for OpenGL to use
    vertexBufferObject.bind();
    vertexBufferObject.createAndInitializeBufferData(vertices);

    // TODO: To which class these should go?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>((3 * sizeof(float))));
    glEnableVertexAttribArray(1);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    vertexBufferObject.unbind();

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
    // happens.
    // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
    // when it's not directly necessary.
    // vertexArrayObject.unbind();


    // render loop
    while (!window.shouldClose())
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // be sure to activate the shader before any calls to glUniform
        shaderProgram.useProgram();

        // render the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        checkErrors();

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
