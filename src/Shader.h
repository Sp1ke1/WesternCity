#pragma once
#include <string> 
#include "pgr.h"
#include "Material.h"
#include <iostream>
class Shader
{
public:
	/**
	 * @brief Constructs a Shader object with the specified shader name.
	 *
	 * @param ShaderName The name of the shader.
	 */
	Shader(const std::string& ShaderName);

	/**
	 * @brief Default constructor for Shader class.
	 *
	 * Constructs a Shader object with default values.
	 */
	Shader() = default;

	/**
	 * @brief Loads the shader program from the specified vertex and fragment shader files.
	 *
	 * @param VSPath The path to the vertex shader file.
	 * @param FSPath The path to the fragment shader file.
	 * @return True if the shader program was loaded successfully, false otherwise.
	 */
	bool LoadShaderFromFile(const std::string& VSPath, const std::string& FSPath);

	/**
	 * @brief Uses the shader program for rendering.
	 */
	void UseShader() const;

	/**
	 * @brief Checks if the shader program is valid and loaded.
	 *
	 * @return True if the shader program is valid and loaded, false otherwise.
	 */
	bool GetIsValid() const;

	/**
	 * @brief Gets the name of the shader.
	 *
	 * @return The name of the shader.
	 */
	std::string GetShaderName() const;

	/**
	 * @brief Binds the material properties to the shader program.
	 *
	 * @param mat The material properties to bind.
	 */
	void BindMaterial(const Material& mat) const;

	/**
	 * @brief Sets a boolean parameter in the shader program.
	 *
	 * @param ParameterName The name of the boolean parameter.
	 * @param Value The value to set.
	 */
	void SetBoolParameter(const std::string& ParameterName, bool Value) const;

	/**
	 * @brief Sets an integer parameter in the shader program.
	 *
	 * @param ParameterName The name of the integer parameter.
	 * @param Value The value to set.
	 */
	void SetIntParameter(const std::string& ParameterName, int Value) const;

	/**
	 * @brief Sets a floating-point parameter in the shader program.
	 *
	 * @param ParameterName The name of the floating-point parameter.
	 * @param Value The value to set.
	 */
	void SetFloatParameter(const std::string& ParameterName, float Value) const;

	/**
	 * @brief Sets a 3D vector parameter in the shader program.
	 *
	 * @param ParameterName The name of the vector parameter.
	 * @param Value The value to set.
	 */
	void SetVec3Parameter(const std::string& ParameterName, const glm::vec3& Value) const;

	/**
	 * @brief Sets a 4D vector parameter in the shader program.
	 *
	 * @param ParameterName The name of the vector parameter.
	 * @param Value The value to set.
	 */
	void SetVec4Parameter(const std::string& ParameterName, const glm::vec4& Value) const;

	/**
	 * @brief Sets a 4x4 matrix parameter in the shader program.
	 *
	 * @param ParameterName The name of the matrix parameter.
	 * @param Value The value to set.
	 */
	void SetMat4Parameter(const std::string& ParameterName, const glm::mat4& Value) const;

	/**
	 * @brief Sets a 3x3 matrix parameter in the shader program.
	 *
	 */
	void SetMat3Parameter(const std::string& ParameterName, const glm::mat3& Value) const;
	
private:
	std::string m_ShaderName; 
	GLuint m_ProgramID; 
	bool m_IsLoaded = false;
};

