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

enum class CameraType
{
    Fly,
    FPS
};

/**
 * @brief
 *
 */
class Camera
{
  public:
    Camera(CameraType cameraType, const glm::vec3 position, const glm::vec3 frontDirection, const glm::vec3 up);
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

    /**
     * @brief Manual look at matrix for learning purposes and exercise 7_2:
     * Try to create your own LookAt function where you manually create a view matrix as discussed at the start of this
     * chapter. Replace glm's LookAt function with your own implementation and see if it still acts the same
     *
     * @return glm::mat4
     */
    glm::mat4 getManualLooAt() const;

    CameraType cameraType_;
    glm::vec3 cameraPosition_;
    glm::vec3 cameraFrontDirection_;
    glm::vec3 worldUp_;
    glm::vec3 cameraRight_;
    glm::vec3 cameraUp_;

    float movementSpeed_;
    float mouseSensitivity_;
    float cameraZoom_;
    float yaw_;
    float pitch_;
};

#endif /* SRC_COMMON_CAMERA */
