#pragma once
#include "InputHandler.h"
#include "Scene.h" 
#include "Config.h"
#include <exception>
#include <iostream>
#include "Misc.h"
#include "pgr.h"

#define DEFAULT_CONFIG_NAME "config.txt" // TODO: move to argc argv

class Application
{
public:
	/**
	 * @brief Initializes the application.
	 *
	 * @param argc The number of command line arguments.
	 * @param argv The command line arguments.
	 * @return True if initialization was successful, false otherwise.
	 */
	static bool Init(int argc, char** argv);

	/**
	 * @brief Starts the main loop of the application.
	 *
	 * @return True if the main loop was started successfully, false otherwise.
	 */
	static bool StartMainLoop();

	/**
	 * @brief Exits the application.
	 */
	static void Exit();

	/**
	 * @brief Updates the application.
	 *
	 * @param Value The value passed from the timer function (glutTimerFunc).
	 */
	static void Update(int Value /*glutTimerFunc cb req*/);

	/**
	 * @brief Callback function for displaying the application's content.
	 */
	static void DisplayCallback();

private:
	/**
	 * @brief Initializes the GLUT library and creates the application's window.
	 *
	 * @param argc The number of command line arguments.
	 * @param argv The command line arguments.
	 * @return True if GLUT initialization was successful, false otherwise.
	 */
	static bool InitGlut(int argc, char** argv);

	/**
	 * @brief Handles input for the application.
	 *
	 * @param dt The time elapsed since the last frame.
	 */
	static void HandleInput(float dt);

	static InputHandler m_InputHandler;  /**< The input handler for the application. */
	static Config m_Config;  /**< The configuration settings for the application. */
	static Scene m_Scene;  /**< The scene of the application. */
	static int m_TargetTickrate;  /**< The target tick rate of the application. */
	static int m_LastTickTime;  /**< The time of the last tick in milliseconds. */
	static glm::vec2 m_WindowSize;  /**< The size of the application's window. */
};

