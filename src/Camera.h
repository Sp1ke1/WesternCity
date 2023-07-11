#pragma once
#include "pgr.h"
#include "Misc.h"
#include "SceneObject.h"
#include <iostream>

#define CAMERA_MOVE_SPEED 4.f; // TODO: move to config 
#define MOUSE_SENSIBILITY 1.f;

/**
 * @brief Represents a camera in the scene.
 *
 * This class extends the SceneObject class and provides functionality for camera movement and manipulation.
 */
class Camera : public SceneObject
{
public:
    bool Static; /**< Flag indicating if the camera is static. */
    bool Movable; /**< Flag indicating if the camera is movable. */
    bool Flashlight; /**< Flag indicating if the flashlight is enabled for the camera. */

    float MovementSpeed; /**< The movement speed of the camera. */

    /**
     * @brief Constructs a Camera object with the specified parameters.
     * @param Name The name of the camera.
     * @param Location The initial location of the camera.
     * @param Pitch The initial pitch angle of the camera.
     * @param Yaw The initial yaw angle of the camera.
     * @param isStatic Flag indicating if the camera is static.
     * @param isMovable Flag indicating if the camera is movable.
     */
    Camera(const std::string& Name, const glm::vec3& Location, float Pitch = 0.0f, float Yaw = 0.0f, bool isStatic = false, bool isMovable = true);

    /**
     * @brief Returns the view matrix of the camera.
     * @return The view matrix.
     */
    glm::mat4 GetViewMatrix();

    /**
     * @brief Returns the projection matrix of the camera.
     * @return The projection matrix.
     */
    glm::mat4 GetProjectionMatrix();

    /**
    * @brief Sets new projection parameters in camera 
    * 
    */
    void SetProjectionParameters(float FOV, float aspect, float zNear, float zFar); 
    /**
     * @brief Toggles the static flag of the camera.
     */
    void ToggleStatic();

    /**
     * @brief Returns the pitch angle of the camera.
     * @return The pitch angle.
     */
    float GetPitch() const;

    /**
     * @brief Returns the yaw angle of the camera.
     * @return The yaw angle.
     */
    float GetYaw() const;

    /**
     * @brief Returns the camera speed.
     * @return The camera speed.
     */
    float GetCameraSpeed() const;

    /**
     * @brief Returns the mouse sensitivity of the camera.
     * @return The mouse sensitivity.
     */
    float GetMouseSensitivity() const;

    /**
     * @brief Processes mouse movement to update the camera's orientation.
     * @param offset The mouse movement offset.
     */
    void ProcessMouseMovement(const glm::vec2& offset);

private:
    glm::mat4 ProjectionMat; /**< The projection matrix of the camera. */
    float m_Speed = CAMERA_MOVE_SPEED; /**< The movement speed of the camera. */
    float m_MouseSensitivity = MOUSE_SENSIBILITY; /**< The mouse sensitivity of the camera. */
    float m_Pitch; /**< The pitch angle of the camera. */
    float m_Yaw; /**< The yaw angle of the camera. */
};