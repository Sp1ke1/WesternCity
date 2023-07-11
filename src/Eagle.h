#pragma once 

#include "GameObject.h" 
#include "pgr.h"


class Scene; 


/**
 * @brief Represents an Eagle object in the scene.
 *
 * This class extends the GameObject class and provides additional functionality specific to an Eagle object.
 */
class Eagle : public GameObject
{
public:
    /**
     * @brief Constructs an Eagle object with the specified parameters.
     * @param Name The name of the Eagle.
     * @param mesh The shared pointer to the Mesh object representing the Eagle's mesh.
     * @param transform The transform of the Eagle.
     */
    Eagle(const std::string& Name, const std::shared_ptr<Mesh>& mesh, const Transform& transform);

    Eagle() = default;

    /**
     * @brief Updates the Eagle object.
     * @param dt The time difference between the current and previous frames.
     */
    virtual void Update(float dt) override;

    /**
     * @brief Switches the animation frames to the next one.
     */
    void SwitchAnimationFrame();

    /**
     * @brief Renders the Eagle object using the specified shader.
     * @param shader The shader to use for rendering.
     */
    void Render(const Shader& shader) override;

    /**
     * @brief Loads the Eagle object from file with the specified base name and suffixes.
     * @param baseName The base name of the file.
     * @param suffixes The vector of suffixes to append to the base name for loading different parts of the Eagle.
     * @return True if loading is successful, false otherwise.
     */
    bool LoadFromFile(const std::string& baseName, const std::vector<std::string>& suffixes);

private:
    size_t CurrentAnimationFrame = 0; /**< The index of the current animation frame. */
    size_t NextAnimationFrame = 1; /**< The index of the next animation frame. */
    float CurrentAnimationAlpha; /**< The alpha value for blending between animation frames. */
    float FrameLength = 0.3f; /**< The length of each animation frame. */
    float FrameStartTime = 0.f; /**< The start time of the current frame. */
    std::vector<std::shared_ptr<Mesh>> m_Meshes; /**< The vector of Mesh objects representing different parts of the Eagle. */
    size_t nOfFaces; /**< The number of faces in the Eagle mesh. */
    size_t nOfVertices; /**< The number of vertices in the Eagle mesh. */
    GLuint m_EagleVAO; /**< The Vertex Array Object (VAO) for the Eagle mesh. */
    GLuint m_EagleVBO; /**< The Vertex Buffer Object (VBO) for the Eagle mesh. */
    GLuint m_EagleEBO; /**< The Element Buffer Object (EBO) for the Eagle mesh. */
    std::vector<Texture> m_EagleTextures; /**< The vector of textures associated with the Eagle mesh. */

    // Eagle parameters
    glm::vec3 EagleCircleCenter = glm::vec3(2.f, 7.f, 0.f); /**< The center position of the Eagle's circular path. */
    float EagleCircleRadius = 4.f; /**< The radius of the Eagle's circular path. */
    float EagleStartPosition = 0.0f; /**< The starting position of the Eagle on the circular path. */
    float EagleAngle = 0.f; /**< The current angle of the Eagle on the circular path. */
    float EagleSpeed = 12.f; /**< The speed at which the Eagle moves along the circular path. */
};