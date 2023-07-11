#pragma once
#include "pgr.h"
#include "glm/gtx/quaternion.hpp"
#include "stb_image.h"
#include <iostream>
#include "Material.h"

/**
 * @brief Represents a transformation in 3D space, including location, rotation, and scale.
 */
struct Transform
{
	glm::vec3 Location; /**< The location of the transform. */
	glm::quat Rotation; /**< The rotation of the transform. */
	glm::vec3 Scale; /**< The scale of the transform. */

	/**
	 * @brief Default constructor for Transform.
	 * Initializes the transform with default values.
	 */
	Transform()
		: Location(0.0f, 0.0f, 0.0f), Rotation(glm::vec3(0.0f, 0.0f, 0.0f)), Scale(1.0f, 1.0f, 1.0f)
	{};

	/**
	 * @brief Constructs a Transform with the specified location, rotation, and scale.
	 *
	 * @param Location The location of the transform.
	 * @param Rotation The rotation of the transform.
	 * @param Scale The scale of the transform.
	 */
	Transform(const glm::vec3& Location, const glm::vec3& Rotation, const glm::vec3& Scale)
		: Location(Location),
		Rotation(glm::vec3(glm::radians(Rotation.x), glm::radians(Rotation.y), glm::radians(Rotation.z))),
		Scale(Scale)
	{
	};

	/**
	 * @brief Converts the Transform to a 4x4 transformation matrix.
	 *
	 * @return The transformation matrix representing the Transform.
	 */
	glm::mat4 ToMat4() const
	{
		glm::mat4 TranslationMat = glm::translate(glm::mat4(), Location);
		glm::mat4 ScaleMat = glm::scale(glm::mat4(), Scale);
		glm::mat4 RotationMat = glm::toMat4(Rotation);

		return TranslationMat * RotationMat * ScaleMat;
	}

	/**
	 * @brief Calculates the inverse of the Transform.
	 *
	 * @return The inverse Transform.
	 */
	Transform Inverse() const
	{
		Transform res;
		res.Location = -this->Location;
		res.Rotation = glm::inverse(this->Rotation);
		res.Scale = this->Scale;
		return res;
	}

	/**
	 * @brief Multiplies two Transforms together.
	 *
	 * @param b The Transform to multiply with.
	 * @return The resulting Transform after multiplication.
	 */
	Transform operator*(const Transform& b)
	{
		Transform res;
		res.Location = this->Location + b.Location;
		res.Rotation = this->Rotation * b.Rotation;
		res.Scale = this->Scale * b.Scale;
		return res;
	}
};

enum InputAction
{
	MoveForward, 
	MoveBackward, 
	MoveLeft,
	MoveRight,
	Fire
};

/**
* @brief Defulats materials used in project
*/
struct MaterialsContainer
{
	Material HouseMaterial{ glm::vec3(0.15f), glm::vec3(0.75f), glm::vec3(0.5f), 42 };
	Material ChurchMaterial{ glm::vec3(0.1f), glm::vec3(0.75f), glm::vec3(0.5f), 36 };
	Material RevolverMaterial{ glm::vec3(0.1f), glm::vec3(0.85f), glm::vec3(0.5f), 32 };
	Material DefaultMaterial{ glm::vec3(0.15f), glm::vec3(0.75f), glm::vec3(0.5f), 48 };
};


/**
* @brief Defulat direction light settings used in project
*/
struct DirectionalLightDefaults
{
	// glm::vec3 direction = glm::vec3(0.6, 0.4f, 0.2f);
	glm::vec3 direction = glm::vec3(-1.0f, 1.0f, -1.0f);

	glm::vec3 ambientColor = glm::vec3(0.25f);
	glm::vec3 diffuseColor = glm::vec3(0.6f);
	glm::vec3 specularColor = glm::vec3(0.6f);
};
/**
* @brief Defulat point light settings used in project
*/
struct PointLightDefaults
{
	glm::vec3 ambientColor = glm::vec3(0.07f);
	glm::vec3 diffuseColor = glm::vec3(0.8f);
	glm::vec3 specularColor = glm::vec3(0.6f);

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

/**
* @brief Defulat spotlight light settings used in project
*/
struct SpotlightDefaults
{
	glm::vec3 ambientColor = glm::vec3(0.25f);
	glm::vec3 diffuseColor = glm::vec3(0.8f);
	glm::vec3 specularColor = glm::vec3(1.0f);

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float cutOff = glm::cos(glm::radians(20.f));
	float exponent = 32.0f;
};


/**
* @brief Defulat night fog settings used in project
*/
struct FogNightDefaults
{
	float maxDistance = 20.f; 
	float minDistance = 0.1f; 
	glm::vec4 color = { 0.3f, 0.3f, 0.3f, 1.f }; 
};

/**
* @brief Defulat day fog settings used in project
*/
struct FogDayDefualts
{
	float maxDistance = 35.f; 
	float minDistance = 0.1f; 
	glm::vec4 color = { 0.5f, 0.5f, 0.5f, 1.0f };
};


