#pragma once

#include <string> 
#pragma once 
#include <iostream>
#include <fstream>
#include <memory>
#include "Camera.h"
#include "GameObject.h"
#include "Shader.h"
#include "Eagle.h"
#include "FunctionLibrary.h"

#define REVOLVER_ID 1
#define CHEST_TOP_ID 2
#define EAGLE_ID 3
#define SKYBOX_PATH "resources/textures/skybox/"
#define SKYBOX_BASE_NAME "sk"
#define SKYBOX_SUFFIXES std::vector<std::string> {"right",  "left", "top", "bottom",  "front", "back"  }
#define SKYBOX_EXTENSION ".jpg"


class Application; 

class Scene
{
	friend Application;  

public:

	friend Application;

public:
	/**
	 * @brief Duration of each frame of the muzzle flash animation.
	 */
	float MuzzleFlashFrameDuration = 0.08f;

	/**
	 * @brief Start time of the muzzle flash animation.
	 */
	float MuzzleFlashStartTime = 0.f;

	/**
	 * @brief Indicates whether the muzzle flash animation is active.
	 * @note TODO: Move to muzzle flash class.
	 */
	bool MuzzleFlashActive = false;

	/**
	 * @brief Current frame of the muzzle flash animation.
	 */
	int MuzzleFlashFrame = 0;

	/**
	 * @brief Total number of frames in the muzzle flash animation.
	 */
	int MuzzleFlashTotalFrames = 16;

	GLint MuzzleFlashTexture; /**< Texture used for the muzzle flash animation. */



	/**
	 * @brief Indicates whether the chest animation is active.
	 */
	bool ChestAnimationActive = false;

	/**
	 * @brief Indicates whether the chest is opened.
	 */
	bool ChestIsOpened = false;

	/**
	 * @brief Duration of the chest animation.
	 */
	float ChestAnimationTime = 1.0f;

	/**
	 * @brief Start time of the chest animation.
	 */
	float ChestAnimationStartTime = 0.f;

	glm::quat ChestRotationClosed = glm::vec3(glm::radians(0.f), glm::radians(90.f), 0.f); /**< Quaternion representing the closed chest rotation. */
	glm::quat ChestRotationOpened = glm::vec3(glm::radians(-45.f), glm::radians(90.f), 0.f);  /**< Quaternion representing the opened chest rotation. */
	glm::quat TargetChestRotation = {}; /**< Quaternion representing the target chest rotation. */
	glm::quat InitialChestRotation = {}; /**< Quaternion representing the initial chest rotation. */

	GLuint SkyboxTexture; /**< Texture ID for the skybox. */

	float DayStartTime; 
	float DayLength = 16.f;
	float NightLength = 8.f;
	bool isNight = false;



	private: 
	/**
		* @brief Updates the scene.
		*
		* @param dt The time elapsed since the last frame.
		*/
	void Update(float dt);

	/**
		* @brief Loads the scene from a file.
		*
		* @param Filename The name of the file to load the scene from.
		* @return True if the scene was loaded successfully, false otherwise.
		*/
	bool LoadSceneFromFile(const std::string& Filename);

	/**
		* @brief Loads the skybox textures.
		*
		* @param BaseName The base name of the skybox textures.
		* @param SkyboxPath The path to the skybox textures.
		* @param Suffixes The suffixes for the skybox textures.
		* @param Extension The extension of the skybox textures.
		* @return True if the skybox textures were loaded successfully, false otherwise.
		*/
	bool LoadSkybox(const std::string& BaseName, const std::string& SkyboxPath, const std::vector<std::string> Suffixes, const std::string& Extension);

	/**
		* @brief Loads the cube mesh.
		*
		* @return True if the cube mesh was loaded successfully, false otherwise.
		*/
	bool LoadCube();

	/**
		* @brief Loads the shaders.
		*
		* @return True if the shaders were loaded successfully, false otherwise.
		*/
	bool LoadShaders();

	/**
		* @brief Loads the eagle model.
		*
		* @return True if the eagle model was loaded successfully, false otherwise.
		*/
	bool LoadEagle();
	
	/**
	* @bief Renders the whole scene
	*/
	void Render();

	/**
 * @brief Renders the skybox.
 */
	void RenderSkybox();

	/**
	 * @brief Renders the water for a given game object.
	 *
	 * @param water The game object representing the water.
	 */
	void RenderWater(std::shared_ptr<GameObject>& water) const;

	/**
	 * @brief Renders the eagle model.
	 */
	void RenderEagle();

	/**
	 * @brief Finds a game object in the scene by its name.
	 *
	 * @param PartialName The partial name of the game object.
	 * @return A shared pointer to the game object if found, nullptr otherwise.
	 */
	std::shared_ptr<GameObject> FindObjectByName(const std::string& PartialName) const;

	/**
	 * @brief Renders a billboard for a given game object.
	 *
	 * @param object The game object to render the billboard for.
	 */
	void RenderBillboard(const std::shared_ptr<GameObject>& object) const;

	/**
	 * @brief Sets the scene-specific uniforms in the shader.
	 *
	 * @param shader The shader to set the uniforms for.
	 */
	void SetSceneUniforms(const Shader& shader);

	/**
	 * @brief Checks if a mesh is already loaded.
	 *
	 * @param Path The path to the mesh file.
	 * @param Index Output parameter for the index of the loaded mesh if found.
	 * @return True if the mesh is already loaded, false otherwise.
	 */
	bool GetIsMeshLoaded(const std::string& Path, size_t& Index) const;

	/**
	 * @brief Processes an input action.
	 *
	 * @param inputAction The input action to process.
	 * @param dt The time elapsed since the last frame.
	 */
	void ProcessInputAction(InputAction inputAction, float dt);

	/**
	 * @brief Processes mouse movement input.
	 *
	 * @param Offset The mouse movement offset.
	 * @param dt The time elapsed since the last frame.
	 */
	void ProcessMouseMovement(const glm::vec2& Offset, float dt);

	/**
	 * @brief Processes mouse click input.
	 *
	 * @param Position The mouse click position.
	 * @param dt The time elapsed since the last frame.
	 */
	void ProcessMouseClick(const glm::vec2& Position, float dt);

	/**
	 * @brief Toggles the spotlight.
	 */
	void ToggleSpotlight();

	/**
	 * @brief Toggles the point lights.
	 */
	void TogglePointLights();

	/**
	 * @brief Toggles camera movement.
	 */
	void ToggleCameraMovement();

	/**
	 * @brief Sets up the cameras for the scene.
	 */
	void SetupCameras();

	/**
	 * @brief Sets up the lights for the scene.
	 */
	void SetupLights();

	/**
	 * @brief Sets the default uniforms for the lights.
	 */
	void SetLightDefaultUniforms();

	/**
	 * @brief Returns the active camera in the scene.
	 *
	 * @return A weak pointer to the active camera.
	 */
	std::weak_ptr<Camera> GetActiveCamera() const;

	void SetCamerasAspectRation(float aspect); 

	/**
	 * @brief Returns the shader with the given name.
	 *
	 * @param ShaderName The name of the shader.
	 * @return The shader with the given name if found, an empty shader otherwise.
	 */
	Shader GetShaderByName(const std::string& ShaderName) const;

	std::vector<Shader> m_Shaders; /**< Vector of shaders used in the scene. */

	std::vector<std::shared_ptr<Mesh>> m_LoadedMeshes; /**< Vector of loaded mesh objects in the scene. */

	std::vector<std::shared_ptr<GameObject>> m_GameObjects; /**< Vector of game objects in the scene. */

	std::vector<std::shared_ptr<Camera>> m_Cameras; /**< Vector of cameras in the scene. */

	std::vector<std::pair<glm::vec3, bool>> m_PointLights; /**< Vector of point lights in the scene. Each pair consists of the light position and a flag indicating whether the light is active. */

	glm::vec3 m_DirectionalLightDirection; /**< Direction of the directional light in the scene. */

	size_t m_ActiveCameraIndex; /**< Index of the active camera in the scene. */

	bool m_isSpotlightActive = true; /**< Flag indicating whether the spotlight is active in the scene. */
	

	
};

