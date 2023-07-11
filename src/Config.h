#pragma once


#include <string>
#include <map>
#include <iostream>
#include <fstream>

#define CONFIG_EMPTY_VARIABLE std::string("")
class Application; 
/**
 * @brief A class representing configuration settings.
 *
 * This class provides methods to retrieve configuration variables from a file and access them as integers or strings.
 */
class Config
{

public:
	/**
	 * @brief Retrieves a configuration variable as an integer.
	 * @param Name The name of the configuration variable.
	 * @param Variable Reference to an integer variable to store the retrieved value.
	 * @return True if the variable was found and successfully retrieved, false otherwise.
	 */
	bool GetVariableAsInt(const std::string& Name, int& Variable) const;

	/**
	 * @brief Retrieves a configuration variable as a string.
	 * @param Name The name of the configuration variable.
	 * @param Variable Reference to a string variable to store the retrieved value.
	 * @return True if the variable was found and successfully retrieved, false otherwise.
	 */
	bool GetVariableAsString(const std::string& Name, std::string& Variable) const;

private:
	/**
	 * @brief Loads the configuration settings from a file.
	 * @param Filename The name of the configuration file.
	 * @return True if the configuration file was successfully loaded, false otherwise.
	 */
	bool LoadConfigFromFile(const std::string& Filename);

	/**
	 * @brief Retrieves a configuration variable by name.
	 * @param Name The name of the configuration variable.
	 * @param Variable Reference to a string variable to store the retrieved value.
	 * @return True if the variable was found and successfully retrieved, false otherwise.
	 */
	bool GetVariableByName(const std::string& Name, std::string& Variable) const;

	std::map<std::string, std::string> m_Variables; /**< The map storing the configuration variables. */

	friend Application;
};
