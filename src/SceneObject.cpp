#include "SceneObject.h"


glm::mat4 SceneObject::GetWorldModelMatrix() const
{
	if (m_AttachParent)
	{
		return m_AttachParent->GetWorldModelMatrix() * m_RelativeTransform.ToMat4(); 
	}
	return m_RelativeTransform.ToMat4();
}

SceneObject::SceneObject(const std::string& Name, const Transform& transform)
	: m_Name(Name), m_RelativeTransform(transform)
{
}

void SceneObject::Update(float dt)
{
}

glm::mat4 SceneObject::GetRelativeModelMatrix() const
{
	return m_RelativeTransform.ToMat4();
}

Transform SceneObject::GetWorldTransform() const
{
	if (m_AttachParent)
	{
		return m_AttachParent->GetWorldTransform() * m_RelativeTransform; 
	}
	return m_RelativeTransform;
}

Transform SceneObject::GetRelativeTransform() const
{
	return m_RelativeTransform;
}

glm::vec3 SceneObject::GetWorldLocation() const
{
	if (m_AttachParent)
	{
		return m_RelativeTransform.Location + m_AttachParent->GetWorldLocation(); 
	}
	return m_RelativeTransform.Location;
}

glm::quat SceneObject::GetWorldRotation() const
{
	if (m_AttachParent)
	{
		return m_AttachParent->GetWorldRotation() * m_RelativeTransform.Rotation; 
	}
	return m_RelativeTransform.Rotation;
}

void SceneObject::AddDeltaLocation(const glm::vec3& deltaLocation)
{
	m_RelativeTransform.Location += deltaLocation;
}

void SceneObject::AddDeltaRotation(const glm::vec3& deltaRotation)
{
	glm::quat deltaQuat = glm::quat(deltaRotation);
	m_RelativeTransform.Rotation = deltaQuat * m_RelativeTransform.Rotation;
}

void SceneObject::SetWorldTransform(const Transform& transform)
{
	if (m_AttachParent)
	{
		Transform parentWorldInverse = m_AttachParent->GetWorldTransform().Inverse();
		m_RelativeTransform = parentWorldInverse * transform; 
		return; 
	}
	m_RelativeTransform = transform;
}

void SceneObject::SetWorldLocation(const glm::vec3& location)
{
	if (m_AttachParent)
	{
		glm::vec3 parentWorldLocation = m_AttachParent->GetWorldLocation(); 
		m_RelativeTransform.Location = location - parentWorldLocation;
		return;
	}
	m_RelativeTransform.Location = location;
}

void SceneObject::SetWorldRotation(const glm::vec3& rotation)
{
	glm::quat rotation_quad(rotation);
	if (m_AttachParent)
	{
		glm::quat parentRotationInversed = glm::inverse(m_AttachParent->GetWorldRotation()); 
		m_RelativeTransform.Rotation = parentRotationInversed * rotation_quad; 
		return; 
	}
	m_RelativeTransform.Rotation = rotation_quad;
}

void SceneObject::SetWorldRotation(const glm::quat& rotation)
{
	if (m_AttachParent)
	{
		glm::quat parentRotationInversed = glm::inverse(m_AttachParent->GetWorldRotation());
		m_RelativeTransform.Rotation = parentRotationInversed * rotation;
		return;
	}
	m_RelativeTransform.Rotation = rotation;
}

void SceneObject::SetRelativeLocation(const glm::vec3& location)
{
	m_RelativeTransform.Location = location;	
}

void SceneObject::SetRelativeRotation(const glm::quat& rotation)
{
	m_RelativeTransform.Rotation = rotation; 
}

void SceneObject::SetRelativeTransform(const Transform& transform)
{
	m_RelativeTransform = transform; 
}

void SceneObject::AttachToObject(const std::shared_ptr<SceneObject>& object)
{
	if ( object )
		m_AttachParent = object; 
}

glm::vec3 SceneObject::GetFrontVector() const
{
	return m_RelativeTransform.Rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 SceneObject::GetUpVector() const
{
	return m_RelativeTransform.Rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 SceneObject::GetLeftVector() const
{
	return m_RelativeTransform.Rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

