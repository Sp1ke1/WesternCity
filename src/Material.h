#pragma once
#include "glm/vec3.hpp"
/**
 * @brief Represents the material properties of an object.
 *
 * The material determines how light interacts with the object's surface.
 */
class Material
{
public:
	/**
	 * @brief Constructs a Material object with the specified properties.
	 *
	 * @param ambient The ambient color of the material.
	 * @param diffuse The diffuse color of the material.
	 * @param specular The specular color of the material.
	 * @param shininess The shininess coefficient of the material.
	 */
	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
		: m_AmbientColor(ambient), m_DiffuseColor(diffuse), m_SpecularColor(specular), m_Shininess(shininess)
	{};

	/**
	 * @brief Default constructor for Material class.
	 *
	 * Constructs a Material object with default values.
	 */
	Material() = default;

	glm::vec3 m_AmbientColor;    /**< The ambient color of the material. */
	glm::vec3 m_DiffuseColor;    /**< The diffuse color of the material. */
	glm::vec3 m_SpecularColor;   /**< The specular color of the material. */
	float m_Shininess;           /**< The shininess coefficient of the material. */
};

