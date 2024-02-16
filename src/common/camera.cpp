#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "window.hpp"

Camera::Camera(CameraType cameraType, const glm::vec3 position, const glm::vec3 frontDirection, const glm::vec3 worldUp)
  : cameraType_{ cameraType }, cameraPosition_{ std::move(position) },
    cameraFrontDirection_{ std::move(frontDirection) }, worldUp_{ std::move(worldUp) }
{
    setDefaultValues();
    calculateCameraVectors();
}

void Camera::setCameraSpeed(float speed) { movementSpeed_ = speed; }
void Camera::setMouseSensitivity(float sensitivity) { mouseSensitivity_ = sensitivity; }

void Camera::processCameraMovement(CameraMovement movement, float deltaTime)
{
    float movementFraction = movementSpeed_ * deltaTime;

    auto cameraFrontDirection = cameraFrontDirection_;


    // if (cameraType_ == CameraType::FPS)
    // {
    //     std::cout << pitch_ << std::endl;
    //     cameraFrontDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    //     cameraFrontDirection.x = static_cast<float>(cos(glm::radians(pitch_)) * cos(glm::radians(yaw_)));
    //     cameraFrontDirection.y = 0;
    //     cameraFrontDirection.z = static_cast<float>(cos(glm::radians(pitch_)) * sin(glm::radians(yaw_)));
    //     // cameraFrontDirection = glm::normalize(cameraFrontDirection);
    // }

    switch (movement)
    {
    case CameraMovement::Forward:
        cameraPosition_ += cameraFrontDirection * movementFraction;
        break;
    case CameraMovement::Backward:
        cameraPosition_ -= cameraFrontDirection * movementFraction;
        break;
    case CameraMovement::Right:
        cameraPosition_ += cameraRight_ * movementFraction;
        break;
    case CameraMovement::Left:
        cameraPosition_ -= cameraRight_ * movementFraction;
        break;
    default:
        break;
    }

    if (cameraType_ == CameraType::FPS)
        cameraPosition_.y = 0.0f;
}

void Camera::processCameraTurn(double mouseXOffset, double mouseYOffset)
{
    yaw_ += static_cast<float>(mouseXOffset) * mouseSensitivity_;
    pitch_ += static_cast<float>(mouseYOffset) * mouseSensitivity_;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    calculateCameraVectors();
}

void Camera::processCameraZoom(double yOffset)
{
    cameraZoom_ -= static_cast<float>(yOffset);

    if (cameraZoom_ < 1.0f)
        cameraZoom_ = 1.0f;

    if (cameraZoom_ > 45.0f)
        cameraZoom_ = 45.0f;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(cameraPosition_, cameraPosition_ + cameraFrontDirection_, cameraUp_);
}

float Camera::getCameraZoom() const { return cameraZoom_; }

Camera::~Camera() { setDefaultValues(); }

void Camera::setDefaultValues()
{
    cameraUp_ = worldUp_;
    cameraRight_ = glm::vec3(0.0f, 0.0f, 0.0f);

    movementSpeed_ = 0.1f;
    mouseSensitivity_ = 0.1f;
    cameraZoom_ = 45.0f;

    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing
    // to the right so we initially rotate a bit to the left.
    yaw_ = -90.0f;
    pitch_ = 0.0f;
}

void Camera::calculateCameraVectors()
{
    // Direction vectors
    auto newFront = glm::vec3(0.0f, 0.0f, 0.0f);
    newFront.x = static_cast<float>(cos(glm::radians(pitch_)) * cos(glm::radians(yaw_)));
    newFront.y = static_cast<float>(sin(glm::radians(pitch_)));
    newFront.z = static_cast<float>(cos(glm::radians(pitch_)) * sin(glm::radians(yaw_)));

    cameraFrontDirection_ = glm::normalize(newFront);

    // normalize the vectors, because their length gets closer to 0 the
    // more you look up or down which results in slower movement.
    cameraRight_ = glm::normalize(glm::cross(cameraFrontDirection_, worldUp_));
    cameraUp_ = glm::normalize(glm::cross(cameraRight_, cameraFrontDirection_));
}
