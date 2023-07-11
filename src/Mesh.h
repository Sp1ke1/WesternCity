#pragma once
#include "MeshGeometry.h"
#include "pgr.h"
#include "Shader.h"
#include <vector>
#include <string> 
#include <iostream>
#include <fstream>


class GameObject; 
class Scene; 

/**
 * @brief Represents a 3D mesh object.
 *
 * The mesh contains geometry data and material properties.
 */
class Mesh
{
	friend GameObject;
	friend Scene;

public:
	/**
	 * @brief Default constructor for Mesh class.
	 *
	 * Constructs a Mesh object with default values.
	 */
	Mesh() = default;

	/**
	 * @brief Loads the mesh data from a file.
	 *
	 * @param filename The path to the mesh file.
	 * @return True if the mesh data was loaded successfully, false otherwise.
	 */
	bool LoadFromFile(const std::string& filename);

	/**
	 * @brief Gets the path of the mesh file.
	 *
	 * @return The path of the mesh file.
	 */
	std::string GetPath() const;

	/**
	 * @brief Renders the mesh using the specified shader.
	 *
	 * @param shader The shader used for rendering.
	 */
	virtual void Render(const Shader& shader);

	/**
	 * @brief Renders the mesh using the default shader.
	 */
	virtual void Render() const;

	/**
	 * @brief Sets the material properties for the mesh.
	 *
	 * @param material The material properties to set.
	 */
	void SetMaterial(const Material& material);

	/**
	 * @brief Gets the material properties of the mesh.
	 *
	 * @return The material properties of the mesh.
	 */
	Material GetMaterial() const;

	/**
	 * @brief Gets the geometry data of the mesh.
	 *
	 * @return A reference to the vector of MeshGeometry objects representing the mesh's geometry.
	 */
	const std::vector<MeshGeometry>& GetMeshGeometry() const;

private:
	/**
	 * @brief Loads the geometry data from an aiNode.
	 *
	 * @param Node The aiNode containing the geometry data.
	 * @param Scene The aiScene object containing the mesh data.
	 * @return True if the geometry data was loaded successfully, false otherwise.
	 */
	bool LoadGeometryFromAINode(const aiNode* Node, const aiScene* Scene);

	std::vector<MeshGeometry> m_Geometry;   /**< The geometry data of the mesh. */
	Material m_Material;                    /**< The material properties of the mesh. */
	std::string m_Path;                     /**< The path of the mesh file. */
	const std::string m_ModelsFolder = "resources/models/";     /**< The folder path for model files. */
	const std::string m_TexturesFolder = "resources/textures/"; /**< The folder path for texture files. */
};

