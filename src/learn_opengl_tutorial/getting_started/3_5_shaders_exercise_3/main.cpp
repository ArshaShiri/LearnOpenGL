// Output the vertex position to the fragment shader using the out keyword and set the fragment's color equal to this
// vertex position (see how even the vertex position values are interpolated across the triangle). Once you managed to
// do this; try to answer the following question: why is the bottom-left side of our triangle black

/*
Answer to the question: Do you know why the bottom-left side is black?
-- --------------------------------------------------------------------
Think about this for a second: the output of our fragment's color is equal to the (interpolated) coordinate of
the triangle. What is the coordinate of the bottom-left point of our triangle? This is (-0.5f, -0.5f, 0.0f). Since the
xy values are negative they are clamped to a value of 0.0f. This happens all the way to the center sides of the
triangle since from that point on the values will be interpolated positively again. Values of 0.0f are of course black
and that explains the black side of the triangle.
*/

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
#include "common/vertex_buffer_layout.hpp"
#include "common/window.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void clearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

// void checkErrors()
// {
//     while (GLenum error = glGetError())
//     {
//         std::cout << error << std::endl;
//     }
// }

int main()
{
    const auto windowWidth = 800;
    const auto windowHeight = 600;

    // TODO: This call silently initializes glad stuff which is not related to window.
    auto window = Window(windowWidth, windowHeight);

    // TODO Having the path like this is quite fragile since it depends on the path from which the binary is run.
    // TODO It is much better if the shader paths are passed as command line arguments.
    const auto vertexShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/3_5_shaders_exercise_3/vertex_shader.fs");
    const auto vertexShader = Shader(ShaderType::Vertex, "", vertexShaderFilePath);

    const auto fragmentShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/3_5_shaders_exercise_3/fragment_shader.fs");
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

    auto vertexBufferObject = VertexBuffer{ vertices };
    auto vertexArrayObject = VertexArray{};
    // vertexArrayObject.bind();

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    auto vertexBufferLayout = VertexBufferLayout{};
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(3);
    vertexArrayObject.addBuffer(vertexBufferObject, vertexBufferLayout);


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
