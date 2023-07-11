#pragma once
#include "pgr.h"
#include "Misc.h"

class Scene; 
class SceneObject
{

public:
	friend Scene; 

	SceneObject(const std::string& Name, const Transform& transform ); 
	SceneObject() = default; 

	virtual void Update(float dt);

	glm::mat4 GetRelativeModelMatrix() const; 
	glm::mat4 GetWorldModelMatrix() const; 


	Transform GetWorldTransform() const; 
	Transform GetRelativeTransform() const; 
	glm::vec3 GetWorldLocation() const;
	glm::quat GetWorldRotation() const; 

	void AddDeltaLocation(const glm::vec3& deltaLocation);
	void AddDeltaRotation(const glm::vec3& deltaRotation);



	void SetWorldTransform(const Transform& transform); 
	void SetWorldLocation(const glm::vec3& location); 
	void SetWorldRotation(const glm::vec3& rotation); 
	void SetWorldRotation(const glm::quat& rotation);
	void SetRelativeLocation(const glm::vec3& location);
	void SetRelativeRotation(const glm::quat& rotation);
	void SetRelativeTransform(const Transform& transform); 

	glm::vec3 GetFrontVector() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetLeftVector() const;
	void AttachToObject(const std::shared_ptr<SceneObject>& object); 
	std::shared_ptr<SceneObject> GetAttachParent() const { return m_AttachParent;  }
	std::string GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }


private:
	std::shared_ptr<SceneObject> m_AttachParent; 
	Transform m_RelativeTransform;	
	std::string m_Name;
};

