#include "Camera.h"
#include "SceneObject.h"



Camera::Camera(const std::string & Name, const glm::vec3& Location, float Pitch, float Yaw,  bool isStatic, bool isMovable)
    : 
    SceneObject ( Name, Transform ( Location, glm::vec3 (Pitch, Yaw, 0.f), glm::vec3(1.0f) ) ), 
    Static(isStatic),
    Movable(isMovable),
    Flashlight(false), 
    m_Pitch ( Pitch ),
    m_Yaw ( Yaw )
{
    ProjectionMat = glm::perspective(glm::radians(45.0f), (float)800 / 600, 0.1f, 1500.f); // TODO: move to config 
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::vec3 WorldLocation = GetWorldLocation();
    return glm::lookAt(WorldLocation, WorldLocation + GetFrontVector(), GetUpVector());
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return ProjectionMat;
}

float Camera::GetCameraSpeed() const
{
    return m_Speed; 
}

void Camera::SetProjectionParameters(float FOV, float aspect, float zNear, float zFar)
{
    ProjectionMat = glm::perspective(glm::radians(FOV), aspect, zNear, zFar);
}

void Camera::ToggleStatic()
{
    Static = !Static;

}

float Camera::GetPitch() const
{
    return m_Pitch;
}

float Camera::GetYaw() const
{
    return m_Yaw;
}

float Camera::GetMouseSensitivity() const
{
    return m_MouseSensitivity;
}

void Camera::ProcessMouseMovement(const glm::vec2 & offset )
{
    glm::quat ResultRotation; 
   
    const float xoffset = offset.x * GetMouseSensitivity() * -1.f;
; 
    const float yoffset = offset.y * GetMouseSensitivity();

    m_Yaw += xoffset; 
    m_Pitch += yoffset; 
    
    // TODO: clamp yaw

    if (m_Pitch > 89.f)
        m_Pitch = 89.f;
    if (m_Pitch < -89.f)
        m_Pitch = -89.f;

    
    glm::vec3 EulerAngles = glm::radians ( glm::vec3(m_Pitch, m_Yaw, 0.f) );
    SetWorldRotation ( glm::quat(EulerAngles) ); 

}
