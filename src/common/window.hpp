#ifndef SRC_WINDOW
#define SRC_WINDOW

#include <functional>
#include <memory>
#include <string>

#include <GLFW/glfw3.h>


class Window
{
  public:
    Window(const int width, const int height, const std::string windowName = "LearnOpenGL");
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    GLFWwindow *getGLFWindowPointer() const;
    void registerInputProcessingCallback(const std::function<void(GLFWwindow *window)> &callback);
    void processInput();

    ~Window();

  private:
    void init();
    void createGLFWindow(const int width, const int height, const std::string &windowName);
    void initializeGlad() const;
    void registerFrameBufferCallback();

    static constexpr uint32_t MAJOR_VERSION_ = 3;
    static constexpr uint32_t MINOR_VERSION_ = 3;

    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window_;
    std::function<void(GLFWwindow *window)> processInputCallback_;
};

#endif /* SRC_WINDOW */
