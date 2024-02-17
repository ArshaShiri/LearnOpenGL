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
    // return getManualLooAt();
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

glm::mat4 Camera::getManualLooAt() const
{
    // We are basically getting the coordinate of our object in the new coordinate system
    // originated on the camera.
    // Camera up: U
    // Camera right: R
    // Camera direction: D
    // Vertex postion

    // Ignoring translation:
    // |Rx Ux Dx|   |x|   |vx|    |x| =          |Rx Ux Dx| * |vx|
    // |Ry Uy Dy| * |y| = |vy| => |y| = transpose|Ry Uy Dy| * |vy|
    // |Rz Uz Dz|   |z|   |vz|    |z| =          |Rz Uz Dz| * |vz|


    // 1st the camera translation matrix which is the negative of the camera position
    // In glm we access elements as mat[col][row] due to column-major layout
    glm::mat4 cameraTranslation(1.0f);
    cameraTranslation[3][0] = -cameraPosition_.x;
    cameraTranslation[3][1] = -cameraPosition_.y;
    cameraTranslation[3][2] = -cameraPosition_.z;

    // We then need a rotation matrix that gives a vertex coordinate in our new coordinate system originated on the
    // camera.
    // The positive z direction is the opposite direction of the camera front direction
    const auto zAxis = glm::normalize(-cameraFrontDirection_);
    const auto xAxis = glm::normalize(glm::cross(glm::normalize(worldUp_), zAxis));
    glm::vec3 yAxis = glm::cross(zAxis, xAxis);

    glm::mat4 rotation(1.0f);
    rotation[0][0] = xAxis.x;
    rotation[1][0] = xAxis.y;
    rotation[2][0] = xAxis.z;
    rotation[0][1] = yAxis.x;
    rotation[1][1] = yAxis.y;
    rotation[2][1] = yAxis.z;
    rotation[0][2] = zAxis.x;
    rotation[1][2] = zAxis.y;
    rotation[2][2] = zAxis.z;

    return rotation * cameraTranslation;
}
