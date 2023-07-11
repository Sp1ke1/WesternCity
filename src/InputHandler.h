#pragma once
#include "pgr.h"
#include <vector>
#include <map>
#include <iostream>
class Application; 

class InputHandler
{
	friend Application; 
	/**
 * @brief Handles the event when a regular key is pressed.
 *
 * @param keyPressed The ASCII value of the pressed key.
 * @param mouseX The x-coordinate of the mouse cursor.
 * @param mouseY The y-coordinate of the mouse cursor.
 */
	static void KeyboardPressed(unsigned char keyPressed, int mouseX, int mouseY);

	/**
	 * @brief Handles the event when a regular key is released.
	 *
	 * @param keyReleased The ASCII value of the released key.
	 * @param mouseX The x-coordinate of the mouse cursor.
	 * @param mouseY The y-coordinate of the mouse cursor.
	 */
	static void KeyboardReleased(unsigned char keyReleased, int mouseX, int mouseY);

	/**
	 * @brief Handles the event when a special key is pressed.
	 *
	 * @param specKeyPressed The code of the pressed special key.
	 * @param mouseX The x-coordinate of the mouse cursor.
	 * @param mouseY The y-coordinate of the mouse cursor.
	 */
	static void SpecialKeyboardPressed(int specKeyPressed, int mouseX, int mouseY);

	/**
	 * @brief Forces a regular key to be considered as released.
	 *
	 * This function is used to simulate a key release event programmatically.
	 *
	 * @param key The ASCII value of the key to be forced down.
	 */
	static void ForceKeyDown(unsigned char key);

	/**
	 * @brief Forces a special key to be considered as released.
	 *
	 * This function is used to simulate a special key release event programmatically.
	 *
	 * @param specKey The code of the special key to be forced down.
	 */
	static void ForceSpecKeyDown(int specKey);

	/**
	 * @brief Handles the event when a special key is released.
	 *
	 * @param specKeyReleased The code of the released special key.
	 * @param mouseX The x-coordinate of the mouse cursor.
	 * @param mouseY The y-coordinate of the mouse cursor.
	 */
	static void SpecialKeyboardReleased(int specKeyReleased, int mouseX, int mouseY);

	/**
	 * @brief Handles the event when the mouse cursor moves.
	 *
	 * @param X The x-coordinate of the new mouse position.
	 * @param Y The y-coordinate of the new mouse position.
	 */
	static void MouseMotion(int X, int Y);

	/**
	 * @brief Sets the size of the application window.
	 *
	 * @param Width The width of the window.
	 * @param Height The height of the window.
	 */
	static void SetWindowSize(int Width, int Height);

	/**
	 * @brief Initializes the InputHandler with the specified window dimensions.
	 *
	 * This function sets up the InputHandler and prepares it to handle input events.
	 * It should be called once at the start of the application.
	 *
	 * @param windowWidth The width of the application window.
	 * @param windowHeight The height of the application window.
	 * @return True if the initialization was successful, false otherwise.
	 */
	static bool Init(int windowWidth, int windowHeight);

	/**
	 * @brief Handles the mouse button event.
	 *
	 * @param buttonPressed The code of the pressed mouse button.
	 * @param buttonState The state of the pressed mouse button.
	 * @param mouseX The x-coordinate of the mouse cursor.
	 * @param mouseY The y-coordinate of the mouse cursor.
	 */
	static void MouseFunc(int buttonPressed, int buttonState, int mouseX, int mouseY);

	/**
	 * @brief Forces a mouse button to be considered as released.
	 *
	 * This function is used to simulate a mouse button released event programmatically.
	 *
	 * @param button The code of the mouse button to be forced down.
	 */
	static void ForceMouseDown(int button);
public:
	/**
 * @brief Checks if a mouse button is currently pressed.
 *
 * @param button The code of the mouse button to check.
 * @return True if the specified mouse button is currently pressed, false otherwise.
 */
	static bool GetIsMousePressed(int button);

	/**
	 * @brief Checks if a regular key is currently pressed.
	 *
	 * @param Key The ASCII value of the key to check.
	 * @return True if the specified key is currently pressed, false otherwise.
	 */
	static bool GetIsKeyPressed(unsigned char Key);

	/**
	 * @brief Checks if a special key is currently pressed.
	 *
	 * @param Key The code of the special key to check.
	 * @return True if the specified special key is currently pressed, false otherwise.
	 */
	static bool GetIsSpecKeyPressed(int Key);

	/**
	 * @brief Checks if a key chord is currently pressed.
	 *
	 * A key chord is a combination of regular and special keys that must be pressed together.
	 *
	 * @param KeyChord The vector of ASCII values of regular keys in the key chord.
	 * @param SpecKeyChord The vector of special key codes in the key chord. (optional)
	 * @return True if all keys in the key chord are currently pressed, false otherwise.
	 */
	static bool GetIsKeyChordPressed(const std::vector<unsigned char>& KeyChord, const std::vector<int>& SpecKeyChord = {});

	/**
	 * @brief Retrieves the offset of the mouse cursor from the last frame.
	 *
	 * The offset represents the change in mouse position since the last frame.
	 *
	 * @return The offset of the mouse cursor as a 2D vector.
	 */
	static glm::vec2 GetMouseOffset();

	/**
	 * @brief Retrieves the last position of the left mouse click (LMC).
	 *
	 * The LMC position represents the last position where the left mouse button was clicked.
	 *
	 * @return The last position of the LMC as a 2D vector.
	 */
	static glm::vec2 GetLastLMCPosition();
private:
	static std::map <int, bool> m_MouseKeys; 
	static std::map <int, bool> m_Keys; 
	static std::map <int, bool> m_SpecKeys; 
	static glm::vec2 m_WindowSize;
	static glm::vec2 m_CurrentMousePosition; 
	static glm::vec2 m_LastMousePosition; 
	static glm::vec2 m_LastLMCPosition; 
};

