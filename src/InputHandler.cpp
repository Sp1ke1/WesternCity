#include "InputHandler.h"


glm::vec2 InputHandler::m_WindowSize = { 800, 600 };
glm::vec2 InputHandler::m_CurrentMousePosition = {};
glm::vec2 InputHandler::m_LastMousePosition = {};
glm::vec2 InputHandler::m_LastLMCPosition = {};
std::map <int, bool> InputHandler::m_Keys = {};
std::map <int, bool> InputHandler::m_SpecKeys = {};
std::map <int, bool> InputHandler::m_MouseKeys = {}; 
void InputHandler::ForceKeyDown(unsigned char key)
{
	m_Keys[key] = false;
}
void InputHandler::ForceSpecKeyDown(int specKey)
{
	m_SpecKeys[specKey] = false;
}
void InputHandler::KeyboardPressed(unsigned char keyPressed, int mouseX, int mouseY)
{
	if (keyPressed == 27) {
		glutLeaveMainLoop();
		exit(EXIT_SUCCESS);
	}

	m_Keys[keyPressed] = true; 

}
glm::vec2 InputHandler::GetMouseOffset()
{
	glm::vec2 WindowCenter = { m_WindowSize.x / 2, m_WindowSize.y / 2 };
	return m_CurrentMousePosition - WindowCenter; 
}

void InputHandler::KeyboardReleased(unsigned char keyReleased, int mouseX, int mouseY)
{
	m_Keys[keyReleased] = false;
}
void InputHandler::SpecialKeyboardPressed(int specKeyPressed, int mouseX, int mouseY)
{
	m_SpecKeys[specKeyPressed] = true; 
}
void InputHandler::SpecialKeyboardReleased(int specKeyReleased, int mouseX, int mouseY)
{
	m_SpecKeys[specKeyReleased] = false;

}

void InputHandler::MouseMotion(int X, int Y)
{
	m_LastMousePosition = m_CurrentMousePosition; 
	m_CurrentMousePosition = { X, Y };
}

glm::vec2 InputHandler::GetLastLMCPosition()
{
	return m_LastLMCPosition;
}

void InputHandler::MouseFunc(int buttonPressed, int buttonState, int mouseX, int mouseY)
{
	m_LastLMCPosition = { mouseX, mouseY };
	m_MouseKeys[buttonPressed] = buttonState == GLUT_DOWN ? true : false; 

}

void InputHandler::ForceMouseDown(int button)
{
	m_MouseKeys[button] = false; 
}
void InputHandler::SetWindowSize(int Width, int Height)
{
	m_WindowSize = { Width, Height };
}

bool InputHandler::GetIsMousePressed(int button)
{
	if (m_MouseKeys.count(button) == 0)
		return false; 
	return m_MouseKeys.at(button); 
}

bool InputHandler::Init(int windowWidth, int windowHeight)
{
	SetWindowSize(windowWidth, windowHeight);
	return true;
}

bool InputHandler::GetIsKeyPressed(unsigned char Key)
{
	if (m_Keys.count(Key) == 0)
		return false;
	return m_Keys.at(Key); 
}

bool InputHandler::GetIsSpecKeyPressed(int Key)
{
	if (m_SpecKeys.count(Key) == 0)
		return false;
	return m_SpecKeys.at(Key); 
}

bool InputHandler::GetIsKeyChordPressed(const std::vector<unsigned char>& Chord, const std::vector <int>& SpecKeyChord)
{
	if (Chord.empty() && SpecKeyChord.empty())
		return false;


	for (auto Key : Chord)
	{
		if (!GetIsKeyPressed(Key))
			return false;
	}
	for (auto SpecKey : SpecKeyChord)
	{
		if (!GetIsSpecKeyPressed(SpecKey))
			return false;
	}

	return true; 
}


