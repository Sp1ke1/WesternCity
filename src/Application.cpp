#include "Application.h"

Config Application::m_Config = {};
Scene Application::m_Scene = {};
InputHandler Application::m_InputHandler = {};
int Application::m_TargetTickrate = 0; 
int Application::m_LastTickTime = 0;
glm::vec2 Application::m_WindowSize = { 800, 600 };




bool Application::Init(int argc, char** argv)
{

	if (!m_Config.LoadConfigFromFile(DEFAULT_CONFIG_NAME)) // TODO: check for argv
	{
		std::cerr << "Application::Init() => Error loading config" << std::endl;
		return false;
	}


	if (!Application::InitGlut(argc, argv))
	{
		std::cerr << "Application::Init() => Error initializing glut" << std::endl;
		return false;
	}

	if (!m_InputHandler.Init((int)m_WindowSize.x, (int)m_WindowSize.y))
	{
		std::cerr << "Application::Init() => Error initializing InputHandler" << std::endl;
		return false;
	}

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
	{
		std::cerr << "Application::Init() => Error initializing pgr framework" << std::endl;
		return false; 
	}
	glEnable(GL_DEPTH_TEST);
	glClearStencil(0);
	std::string SceneFilename; 
	if (!m_Config.GetVariableAsString("SCENE_FILENAME", SceneFilename))
	{
		std::cerr << "Application::Init() => Error getting SCENE_FILENAME config variable" << std::endl;
		return false;
	}
		

	if (!m_Scene.LoadSceneFromFile(SceneFilename))
	{
		std::cerr << "Application::Init() => Error loading scene" << std::endl;
		return false;
	}
	m_Scene.SetCamerasAspectRation(m_WindowSize.x / m_WindowSize.y);
	return true; 
}

bool Application::InitGlut(int argc, char** argv)
{
	
	glutInit(&argc, argv);
	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	//   initial window size + title
	int window_width, window_height;
	std::string window_title; 
	if (!m_Config.GetVariableAsInt("WINDOW_WIDTH", window_width)
		|| !m_Config.GetVariableAsInt("WINDOW_HEIGHT", window_height)
		|| !m_Config.GetVariableAsString("WINDOW_TITLE", window_title)
		)
	{
		std::cerr << "Application::InitGlut() => Can't get config window variables" << std::endl; 
		return false;
	}
	m_WindowSize = { window_width, window_height };

	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(window_title.c_str());
	glutDisplayFunc(&Application::DisplayCallback); 

	// Inputs 
	glutKeyboardFunc( &InputHandler::KeyboardPressed );
	glutKeyboardUpFunc(&InputHandler::KeyboardReleased); 
	glutSpecialFunc(&InputHandler::SpecialKeyboardPressed); 
	glutSpecialUpFunc(&InputHandler::SpecialKeyboardReleased);
	glutPassiveMotionFunc(&InputHandler::MouseMotion);
	glutMouseFunc(&InputHandler::MouseFunc);

	glutCloseFunc(&Application::Exit);

	return true;
}

void Application::HandleInput( float dt )
{
	if (m_InputHandler.GetIsKeyPressed('w'))
		m_Scene.ProcessInputAction(InputAction::MoveForward, dt);

	if (m_InputHandler.GetIsKeyPressed('s'))
		m_Scene.ProcessInputAction(InputAction::MoveBackward, dt); 
	
	if (m_InputHandler.GetIsKeyPressed('a'))
		m_Scene.ProcessInputAction(InputAction::MoveLeft, dt); 

	if (m_InputHandler.GetIsKeyPressed('d'))
		m_Scene.ProcessInputAction(InputAction::MoveRight, dt);

	if (m_InputHandler.GetIsKeyPressed('c')) {
		m_InputHandler.ForceKeyDown('c');
		m_Scene.ToggleSpotlight();
	}
	if (m_InputHandler.GetIsKeyPressed('v')) {
		m_InputHandler.ForceKeyDown('v');
		m_Scene.TogglePointLights();
	}
	if ( m_InputHandler . GetIsKeyChordPressed ( { 'b' }, { GLUT_KEY_ALT_L } ) )
	{
		m_InputHandler.ForceKeyDown('b'); 
		m_InputHandler.ForceSpecKeyDown(GLUT_KEY_ALT_L);
		m_Scene.ToggleCameraMovement(); 
	}
	if (m_InputHandler.GetIsKeyPressed('1')) {
		m_InputHandler.ForceKeyDown('1');
		m_Scene.m_ActiveCameraIndex = 0;
	}
	else if (m_InputHandler.GetIsKeyPressed('2')) {
		m_InputHandler.ForceKeyDown('2');
		m_Scene.m_ActiveCameraIndex = 1;
	}
	else if (m_InputHandler.GetIsKeyPressed('3')) {
		m_InputHandler.ForceKeyDown('3');
		m_Scene.m_ActiveCameraIndex = 2;
	}
	else if (m_InputHandler.GetIsKeyPressed('4')) {
		m_InputHandler.ForceKeyDown('4');
		m_Scene.m_ActiveCameraIndex = 3;
	}

	if (m_InputHandler.GetIsMousePressed(GLUT_LEFT_BUTTON))
	{
		m_InputHandler.ForceMouseDown(GLUT_LEFT_BUTTON); 
		m_Scene.ProcessInputAction(InputAction::Fire, dt); 
		auto LMCPosition = m_InputHandler.GetLastLMCPosition();
		m_Scene.ProcessMouseClick( { LMCPosition.x, m_WindowSize.y -  LMCPosition.y }, dt);
	}

	m_Scene.ProcessMouseMovement(m_InputHandler.GetMouseOffset(), dt);
	glutWarpPointer((int)m_WindowSize.x / 2, (int)m_WindowSize.y / 2);
}

bool Application::StartMainLoop()
{
	int TargetTickRate; 
	if ( !m_Config.GetVariableAsInt("TARGET_TICKRATE", TargetTickRate) )
	{
		std::cerr << "Application::StartMainLoop() => Can't get config target tickrate" << std::endl;
		return false;
	}
	Application::m_TargetTickrate = TargetTickRate; 
	std::cout << "Starting glut main loop" << std::endl; 
	glutWarpPointer((int)m_WindowSize.x / 2, (int)m_WindowSize.y / 2);
	glutTimerFunc(TargetTickRate, &Application::Update, 0);
	glutMainLoop(); 
	return true;
}

void Application::Update( int Value )
{
	float dt = (glutGet(GLUT_ELAPSED_TIME) - m_LastTickTime) * 0.001f; 
	if (dt < 0.1)
	{
		Application::HandleInput(dt);
		m_Scene.Update(dt);
	}
	


	// and plan a new even
	glutTimerFunc(Application::m_TargetTickrate, &Application::Update, 0);

	// create display event
	glutPostRedisplay();
	m_LastTickTime = glutGet(GLUT_ELAPSED_TIME);
}

void Application::DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	m_Scene.Render(); 
	glutSwapBuffers();
}

void Application::Exit()
{

}