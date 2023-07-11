#pragma once
#include "pgr.h"
#include "Shader.h"
#include "Misc.h"
#include "FunctionLibrary.h"
#include <iostream>

struct Vertex
{
	glm::vec3 Location; 
	glm::vec3 Normal; 
	glm::vec2 TextureCoords; 
};

struct Texture
{
	unsigned int Id; 
	std::string Type;
	std::string Path; 
};
class Mesh;
class Scene; 
class GameObject; 
class Eagle; 
class MeshGeometry
{
	friend Mesh;
	friend Scene;
	friend GameObject;
	friend Eagle;

public:
	/**
	 * @brief Default constructor for MeshGeometry.
	 * Initializes the MeshGeometry with default values.
	 */
	MeshGeometry() = default;

	/**
	 * @brief Loads the geometry data from an aiMesh.
	 *
	 * @param Mesh The aiMesh to load the data from.
	 * @param Scene The aiScene containing the mesh.
	 * @return True if the loading is successful, false otherwise.
	 */
	bool LoadFromAiMesh(const aiMesh* Mesh, const aiScene* Scene);

	/**
	 * @brief Checks if a texture is loaded.
	 *
	 * @param Path The path to the texture.
	 * @param Index The index of the loaded texture if found.
	 * @return True if the texture is loaded, false otherwise.
	 */
	static bool GetIsTextureLoaded(const std::string& Path, size_t& Index);

	/**
	 * @brief Renders the mesh geometry using the specified shader.
	 *
	 * @param shader The shader to use for rendering.
	 */
	void Render(const Shader& shader) const;

	/**
	 * @brief Renders the mesh geometry.
	 */
	void Render() const;

	/**
	 * @brief Retrieves the vertex data of the mesh geometry.
	 *
	 * @return The vector of vertices.
	 */
	std::vector<Vertex> GetVerticisData() const;

	/**
	 * @brief Retrieves the index data of the mesh geometry.
	 *
	 * @return The vector of indices.
	 */
	std::vector<unsigned int> GetIndicisData() const;

	/**
	 * @brief Retrieves the texture data of the mesh geometry.
	 *
	 * @return The vector of textures.
	 */
	std::vector<Texture> GetTextureData() const;

private:
	/**
	 * @brief Binds the textures of the mesh geometry to the specified shader.
	 *
	 * @param shader The shader to bind the textures to.
	 */
	void BindTextures(const Shader& shader) const;

	/**
	 * @brief Unbinds the textures of the mesh geometry.
	 */
	void UnbindTextures() const;

	/**
	 * @brief Loads the geometry data from an aiMesh.
	 *
	 * @param Mesh The aiMesh to load the geometry data from.
	 * @return True if the loading is successful, false otherwise.
	 */
	bool LoadGeometryFromAiMesh(const aiMesh* Mesh);

	/**
	 * @brief Loads the materials and textures from an aiMesh.
	 *
	 * @param Mesh The aiMesh to load the materials and textures from.
	 * @param Scene The aiScene containing the mesh.
	 * @return True if the loading is successful, false otherwise.
	 */
	bool LoadMaterialsFromAiMesh(const aiMesh* Mesh, const aiScene* Scene);

	/**
	 * @brief Loads the material textures from an aiMaterial.
	 *
	 * @param Material The aiMaterial to load the textures from.
	 * @param TextureType The type of texture to load.
	 * @param TypeName The name of the texture type.
	 */
	void LoadMaterialTexturesFromAiMaterial(const aiMaterial* Material, aiTextureType TextureType, const std::string& TypeName);

	/**
	 * @brief Loads the geometry data to the GPU.
	 */
	void LoadGeometryToGPU();
	

	std::vector <unsigned int> m_Indicis;
	std::vector <Vertex> m_Vertices; 
	std::vector <Texture> m_Textures; 
	static std::vector <Texture> LoadedTextures; 
	const std::string m_TexturesFolder = "resources/textures/"; 
	const std::string m_ModelsFolder = "resources/models/";
	GLuint VAO; 
	GLuint VBO; 
	GLuint EBO; 
	bool m_IsLoaded = false;
};

