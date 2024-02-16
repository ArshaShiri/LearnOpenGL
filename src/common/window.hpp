#ifndef SRC_WINDOW
#define SRC_WINDOW

#include <functional>
#include <memory>
#include <string>

#include <GLFW/glfw3.h>


/**
 * @brief A window class to wrap glfw window.
 *
 */
class Window
{
  public:
    Window(const int width, const int height, const std::string windowName = "LearnOpenGL");
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    GLFWwindow *getGLFWindowPointer() const;
    void registerInputProcessingCallback(const std::function<void(GLFWwindow *window)> &callback);
    void registerMouseProcessingCallback(void (*callback)(GLFWwindow *, double, double));
    void registerMouseScrollingCallback(void (*callback)(GLFWwindow *, double, double));
    void processInput();
    bool shouldClose() const;
    void swapBuffers();
    int getWidth() const;
    int getHeight() const;

    /**
     * @brief Enable capturing the cursor. Capturing a cursor means that, once the application has focus, the mouse
     * cursor stays within the center of the window (unless the application loses focus or quits).
     *
     */
    void captureMouseInput() const;

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
    int width_;
    int height_;
};

#endif /* SRC_WINDOW */
