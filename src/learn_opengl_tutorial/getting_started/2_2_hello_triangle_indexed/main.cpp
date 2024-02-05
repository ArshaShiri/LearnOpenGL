#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
// Do not change the order of include files here...
#include <GLFW/glfw3.h>

#include "common/index_buffer.hpp"
#include "common/program.hpp"
#include "common/shader.hpp"
#include "common/vertex_array.hpp"
#include "common/vertex_buffer.hpp"
#include "common/window.hpp"

namespace
{

std::string vertexShaderSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";


std::string fragmentShaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";

} // namespace

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

    const auto shaderProgram = Program();
    shaderProgram.attachShader(vertexShader.getShaderId());
    shaderProgram.attachShader(fragmentShader.getShaderId());

    shaderProgram.linkProgram();

    vertexShader.deleteShader();
    fragmentShader.deleteShader();

    // TODO: For now leave it like this until design of a better wrapper for vertices.
    // set up vertex data (and buffer(s)) and configure vertex attributes

    std::vector<float> rectangleVertices{
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f // top left
    };

    std::vector<unsigned int> rectangleVerticesIndices{
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    auto vertexBufferObject = VertexBuffer{ rectangleVertices };
    auto elementBufferObject = IndexBuffer();
    auto vertexArrayObject = VertexArray{};

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // vertexArrayObject.bind();
    auto vertexBufferLayout = VertexBufferLayout{};
    vertexBufferLayout.push<float>(3);

    vertexArrayObject.addBuffer(vertexBufferObject, vertexBufferLayout);

    // 0. copy our vertices array in a buffer for OpenGL to use
    // vertexBufferObject.bind();
    // vertexBufferObject.createAndInitializeBufferData(rectangleVertices);

    elementBufferObject.bind();
    elementBufferObject.createAndInitializeBufferData(rectangleVerticesIndices);

    // TODO: To which class these should go?
    // 1. then set the vertex attributes pointers
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    // glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    vertexBufferObject.unbind();

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    // keep the EBO bound.

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely
    // happens.

    // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
    // when it's not directly necessary.
    vertexArrayObject.unbind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!window.shouldClose())
    {
        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        shaderProgram.useProgram();

        // seeing as we only have a single VAO there's no need to bind it every time, but
        // we'll do so to keep things a bit more organized
        vertexArrayObject.bind();

        // Use glDrawElements to indicate we want to render the triangles from an index buffer.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        checkErrors();
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        window.swapBuffers();
        glfwPollEvents();
    }

    return 0;
}
