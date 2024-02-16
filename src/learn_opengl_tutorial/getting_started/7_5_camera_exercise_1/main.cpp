// See if you can transform the camera class in such a way that it becomes a true fps camera where you cannot fly; you
// can only look around while staying on the xz plane

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

#include "common/camera.hpp"
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
      "src/learn_opengl_tutorial/getting_started/7_5_camera_exercise_1/vertex_shader.vs");
    const auto vertexShader = Shader(ShaderType::Vertex, "", vertexShaderFilePath);

    const auto fragmentShaderFilePath = std::filesystem::current_path().append(
      "src/learn_opengl_tutorial/getting_started/7_5_camera_exercise_1/fragment_shader.fs");
    const auto fragmentShader = Shader(ShaderType::Fragment, "", fragmentShaderFilePath);

    auto shaderProgram = Program();
    shaderProgram.attachShader(vertexShader.getShaderId());
    shaderProgram.attachShader(fragmentShader.getShaderId());
    shaderProgram.linkProgram();
    vertexShader.deleteShader();
    fragmentShader.deleteShader();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // clang-format off
    std::vector<float> vertices{
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // clang-format on

    auto vertexBufferObject = VertexBuffer{ vertices };
    auto vertexArrayObject = VertexArray{};

    auto vertexBufferLayout = VertexBufferLayout{};
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(2);
    vertexArrayObject.addBuffer(vertexBufferObject, vertexBufferLayout);
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

    std::vector<glm::vec3> cubePositions{ glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                          glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                          glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                          glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                          glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f) };

    const auto cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    const auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    const auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    static Camera camera(CameraType::FPS, cameraPosition, cameraFront, cameraUp);
    camera.setCameraSpeed(8.0f);

    // time between current frame and last frame
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    auto processInputCallback = [&deltaTime](GLFWwindow *glfwWindow) {
        if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(glfwWindow, true);

        // float cameraSpeed = static_cast<float>(8.0f * deltaTime);

        if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.processCameraMovement(CameraMovement::Forward, deltaTime);
        }

        if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.processCameraMovement(CameraMovement::Backward, deltaTime);
        }

        if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.processCameraMovement(CameraMovement::Right, deltaTime);
        }

        if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.processCameraMovement(CameraMovement::Left, deltaTime);
        }
    };

    window.registerInputProcessingCallback(processInputCallback);

    // Mouse Logic
    window.captureMouseInput();
    camera.setMouseSensitivity(0.1f);

    // Some of the vars has to be static so the lambda can be converted to function pointer
    static float mouseLastX = windowWidth / 2.0f;
    static float mouseLastY = windowHeight / 2.0f;
    static bool firstMouse = true;
    auto mouseCallback = [](__attribute__((unused)) GLFWwindow *glfwWindow, double mouseX, double mouseY) {
        if (firstMouse)
        {
            mouseLastX = static_cast<float>(mouseX);
            mouseLastY = static_cast<float>(mouseY);
            firstMouse = false;
        }

        auto mouseXOffset = static_cast<float>(mouseX) - mouseLastX;
        auto mouseYOffset = static_cast<float>(mouseY) - mouseLastY;
        mouseLastX = static_cast<float>(mouseX);
        mouseLastY = static_cast<float>(mouseY);

        camera.processCameraTurn(mouseXOffset, mouseYOffset);
    };

    window.registerMouseProcessingCallback(mouseCallback);


    auto scrollCallback = [](__attribute__((unused)) GLFWwindow *glfwWindow,
                             __attribute__((unused)) double xoffset,
                             double yoffset) { camera.processCameraZoom(yoffset); };

    window.registerMouseScrollingCallback(scrollCallback);

    // render loop
    while (!window.shouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind Texture
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_2D, texture2));

        const auto viewMatrix = camera.getViewMatrix();
        shaderProgram.setMatrix4fv("view", viewMatrix);

        auto projection = glm::mat4(1.0f);
        projection = glm::perspective(
          glm::radians(camera.getCameraZoom()), static_cast<float>(windowWidth) / windowHeight, 0.1f, 100.0f);
        shaderProgram.setMatrix4fv("projection", projection);

        vertexArrayObject.bind();

        // render boxes
        for (size_t cubePositionIndex{ 0 }; cubePositionIndex < cubePositions.size(); ++cubePositionIndex)
        {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[cubePositionIndex]);
            float angle = 20.0f * static_cast<float>(cubePositionIndex);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderProgram.setMatrix4fv("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
