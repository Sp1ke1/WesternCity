#pragma once
#include "Mesh.h"
#include "pgr.h"
#include "Shader.h"
#include "Misc.h"
#include "SceneObject.h"
#include <string>


class Scene; 

/**
 * @brief A game object representing an entity in the scene.
 *
 * This class extends the base SceneObject class and adds functionality specific to game objects.
 */
class GameObject : public SceneObject
{
	friend Scene;

public:
	/**
	 * @brief Default constructor for the GameObject class.
	 */
	GameObject() = default;

	/**
	 * @brief Constructor for the GameObject class.
	 * @param Name The name of the game object.
	 * @param Mesh The shared pointer to the mesh associated with the game object.
	 * @param transform The transform of the game object.
	 */
	GameObject(const std::string& Name, const std::shared_ptr<Mesh>& Mesh, const Transform& transform);

	/**
	 * @brief Renders the game object using the specified shader.
	 * @param shader The shader used for rendering.
	 */
	virtual void Render(const Shader& shader);

	/**
	 * @brief Renders the game object.
	 */
	virtual void Render();

	/**
	 * @brief Sets the visibility of the game object.
	 * @param isVisible Boolean value indicating the visibility status.
	 */
	void SetVisibility(bool isVisible);

	/**
	 * @brief Checks if the game object is visible.
	 * @return True if the game object is visible, false otherwise.
	 */
	bool GetIsVisible() const;

private:
	bool m_IsVisible = true; /**< Flag indicating whether the game object is visible. */
	std::shared_ptr<Mesh> m_Mesh; /**< The shared pointer to the mesh associated with the game object. */
};

