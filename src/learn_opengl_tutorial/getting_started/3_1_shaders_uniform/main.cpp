#include <cmath>
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


const std::string vertexShaderSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos, 1.0);\n"
  "}\0";

const std::string fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "uniform vec4 ourColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = ourColor;\n"
  "}\n\0";

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

    const auto vertexShader = Shader(ShaderType::Vertex, vertexShaderSource);
    const auto fragmentShader = Shader(ShaderType::Fragment, fragmentShaderSource);

    auto shaderProgram = Program();
    shaderProgram.attachShader(vertexShader.getShaderId());
    shaderProgram.attachShader(fragmentShader.getShaderId());

    shaderProgram.linkProgram();

    vertexShader.deleteShader();
    fragmentShader.deleteShader();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices{
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f // top
    };

    auto vertexBufferObject = VertexBuffer{};
    auto vertexArrayObject = VertexArray{};

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    vertexArrayObject.bind();

    // 0. copy our vertices array in a buffer for OpenGL to use
    vertexBufferObject.bind();
    vertexBufferObject.createAndInitializeBufferData(vertices);

    // TODO: To which class these should go?
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


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

        // update shader uniform
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);

        // ourColor is defined in the shader program
        const auto vertexColorLocation = shaderProgram.getUniformLocation("ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

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
