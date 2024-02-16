#ifndef SRC_COMMON_CAMERA
#define SRC_COMMON_CAMERA

#include <glm/glm.hpp>

class Window;

enum class CameraMovement
{
    Forward,
    Backward,
    Left,
    Right
};

/**
 * @brief
 *
 */
class Camera
{
  public:
    Camera(const glm::vec3 position, const glm::vec3 frontDirection, const glm::vec3 up);
    void setCameraSpeed(float speed);
    void setMouseSensitivity(float sensitivity);

    glm::mat4 getViewMatrix() const;
    float getCameraZoom() const;

    /**
     * @brief Process camera movement based on a delta time and movement speed
     *
     * @param movement
     * @param deltaTime
     */
    void processCameraMovement(CameraMovement movement, float deltaTime);

    void processCameraTurn(double mouseXOffset, double mouseYOffset);
    void processCameraZoom(double yOffset);

    ~Camera();

  private:
    void setDefaultValues();
    void calculateCameraVectors();

    glm::vec3 cameraPosition_;
    glm::vec3 cameraFrontDirection_;
    glm::vec3 cameraRight_;
    glm::vec3 cameraUp_;
    glm::vec3 worldUp_;

    float movementSpeed_;
    float mouseSensitivity_;
    float cameraZoom_;
    float yaw_;
    float pitch_;
};

#endif /* SRC_COMMON_CAMERA */
