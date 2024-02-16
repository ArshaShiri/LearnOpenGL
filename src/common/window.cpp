#include <iostream>
#include <stdexcept>

#include <glad/glad.h>

#include "window.hpp"

namespace
{

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInputDefault(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

} // namespace

Window::Window(const int width, const int height, const std::string windowName)
  : window_{ nullptr, glfwDestroyWindow }, processInputCallback_{ processInputDefault }
{
    try
    {
        init();
        createGLFWindow(width, height, windowName);
        initializeGlad();
    }
    catch (const std::exception &e)
    {
        std::cout << "Window exception with message: '" << e.what() << "'\n";
    }
    registerFrameBufferCallback();
}

GLFWwindow *Window::getGLFWindowPointer() const { return window_.get(); }

void Window::registerInputProcessingCallback(const std::function<void(GLFWwindow *window)> &callback)
{
    processInputCallback_ = callback;
}

void Window::registerMouseProcessingCallback(void (*callback)(GLFWwindow *, double, double))
{
    glfwSetCursorPosCallback(window_.get(), callback);
}

void Window::registerMouseScrollingCallback(void (*callback)(GLFWwindow *, double, double))
{
    glfwSetScrollCallback(window_.get(), callback);
}

void Window::processInput() { processInputCallback_(window_.get()); }

bool Window::shouldClose() const { return glfwWindowShouldClose(getGLFWindowPointer()); }

void Window::swapBuffers() { glfwSwapBuffers(getGLFWindowPointer()); }

void Window::captureMouseInput() const { glfwSetInputMode(window_.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

Window::~Window() { glfwTerminate(); }

void Window::init()
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION_);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // For macOS
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Window::createGLFWindow(const int width, const int height, const std::string &windowName)
{
    GLFWwindow *window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    window_ = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>(window, glfwDestroyWindow);
    glfwMakeContextCurrent(window_.get());
}

void Window::initializeGlad() const
{
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Window::registerFrameBufferCallback()
{
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    auto frameBufferSizeCallback =
      +[](__attribute__((unused)) GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); };
    glfwSetFramebufferSizeCallback(window_.get(), frameBufferSizeCallback);
}
