#include "Scene.h"

bool Scene::LoadSceneFromFile(const std::string& Filename )
{
	std::ifstream f(Filename);
	if (!f)
	{
		std::cerr << "Scene::LoadFromFile() => Can't open scene filename with given name: " << Filename << std::endl;
		return false; 
	}
	MaterialsContainer materials;

	std::string ObjectName; 
	while (f >> ObjectName)
	{
		glm::vec3 ObjectLocation, ObjectRotation, ObjectScale;
		std::string ModelName;
		if (!(f >> ObjectLocation.x >> ObjectLocation.y >> ObjectLocation.z
			>> ObjectRotation.x >> ObjectRotation.y >> ObjectRotation.z
			>> ObjectScale.x >> ObjectScale.y >> ObjectScale.z >> ModelName)) {
			std::cerr << "Scene::LoadFromFile() => Error loading gameobject with name: " << ObjectName << std::endl;
			continue;
		}

		Material ObjectMaterial = {}; // TODO: to level txt not hardcode 
		if (ObjectName.rfind("Revolver", 0) == 0)
			ObjectMaterial = materials.RevolverMaterial;
		else if (ObjectName.rfind("Church", 0) == 0)
			ObjectMaterial = materials.ChurchMaterial;
		else if (ObjectName.rfind("House", 0) == 0)
			ObjectMaterial = materials.HouseMaterial;
		else
			ObjectMaterial = materials.DefaultMaterial; 


		std::shared_ptr<Mesh> ObjectMesh = nullptr; 
		size_t MeshIndex = 0; 
		if (GetIsMeshLoaded(ModelName, MeshIndex))
		{
			ObjectMesh = m_LoadedMeshes[MeshIndex]; 
		}
		else
		{
			ObjectMesh = std::make_shared<Mesh>(); 
			if (!ObjectMesh->LoadFromFile(ModelName))
			{
				std::cerr << "Scene::LoadSceneFromFile() => Erorr loading mesh of object with name: " << ObjectName << std::endl;
				continue;
			}
			ObjectMesh->SetMaterial(ObjectMaterial); 
			m_LoadedMeshes.push_back(ObjectMesh); 
		}
		Transform ObjectTransform(ObjectLocation, ObjectRotation, ObjectScale); 
		std::shared_ptr<GameObject> NewObject = std::make_shared <GameObject>(ObjectName, ObjectMesh, ObjectTransform ) ;
		m_GameObjects.push_back(std::move(NewObject));
		std::cout << "Succesfully loaded object: " + ObjectName << std::endl; 

	}
	if (!LoadShaders())
		return false;

	MuzzleFlashTexture = TextureFromFile("resources/textures/muzzle_flash.png");
	if (!LoadSkybox(SKYBOX_BASE_NAME, SKYBOX_PATH, SKYBOX_SUFFIXES, SKYBOX_EXTENSION))
	{
		std::cerr << "Scene::LoadSceneFromFile():: Error loading skybox" << std::endl; 
		return false; 
	}

	if (!LoadCube())
	{
		std::cerr << "Scene::LoadSceneFromFile() :: Error loading hard code box cube " << std::endl; 
		return false;
	}

	if (!LoadEagle())
	{
		std::cerr << "Scene:LoadSceneFromFile() Error => failed to load eagle" << std::endl; 
		return false;
	
	}

	SetupCameras(); 
	SetupLights(); 
	DayStartTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f; 

	auto Eagle = FindObjectByName("Eagle"); 
	auto MuzzleFlash = FindObjectByName("muzzle_flash"); 
	if (Eagle && MuzzleFlash)
	{
		MuzzleFlash->AttachToObject(Eagle); 
	}
	return true; 

}

bool Scene::LoadSkybox(const std::string& BaseName, const std::string& SkyboxPath, const std::vector<std::string> Suffixes, const std::string& Extension)
{
	GLuint ID; 
	glGenTextures(1, &ID); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID); 

	int w, h, channels; 

	for (size_t i = 0; i < Suffixes.size(); i ++ )
	{
		std::string suff = Suffixes[i]; 
		std::string path = SkyboxPath + BaseName + suff + Extension; 
		unsigned char* imageData = stbi_load(path.c_str(), &w, &h, &channels, 0); 

		if (!imageData)
		{
			std::cerr << "Scene::LoadSkybox() Error: can't load skybox texture with given suffixs: " << suff << " full path: " << path << std::endl; 
			return false;
			stbi_image_free(imageData); 
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); 
		stbi_image_free(imageData); 
		CHECK_GL_ERROR(); 
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); 
	CHECK_GL_ERROR();

	SkyboxTexture = ID; 
	return true;
}

bool Scene::LoadCube()
{

#pragma region cube_geometry
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	std::vector <Vertex> cubeVertices; 
	for (int i = 0; i < sizeof(vertices) / sizeof( float ); i += 8)
	{
		glm::vec3 location = { vertices[i], vertices[i + 1], vertices[i + 2] };
		glm::vec3 normal = { vertices[i + 3], vertices[i + 4], vertices[i + 5] };
		glm::vec2 uv = { vertices[i + 6], vertices[i + 7] }; 
		Vertex v = { location, normal, uv }; 
		cubeVertices.push_back(v);
	}
#pragma endregion 

	Transform CubeTransform = { { 2.75f, 0.3f, -0.5f, }, { 0.0f, 30.0f, 0.0f, }, { 0.5f, 0.5f, 0.5f } };
	std::shared_ptr <GameObject> CubeObject = std::make_shared<GameObject> () ;
	CubeObject->m_Name = "Box";
	CubeObject->SetWorldTransform(CubeTransform); 
	std::shared_ptr<Mesh> CubeMesh = std::make_shared<Mesh>();
	CubeObject->m_Mesh = CubeMesh;
	MaterialsContainer mats; 
	CubeMesh->m_Material = mats.DefaultMaterial;
	MeshGeometry cubeGeometry; 
	cubeGeometry.m_Vertices = cubeVertices; 

	cubeGeometry.LoadGeometryToGPU(); 
	CHECK_GL_ERROR();

	std::string texture_diffuse_path = "resources/textures/box_diffuse.png"; 
	std::string texture_specular_path = "resources/textures/box_specular.png";
	std::string texture_diffuse_type = "texture_diffuse";
	std::string texture_specular_type = "texture_specular";
	GLuint cubeDiffuse = TextureFromFile("resources/textures/box_diffuse.png");
	GLuint cubeSpecular = TextureFromFile("resources/textures/box_specular.png");

	if (cubeDiffuse == 0 || cubeSpecular == 0)
		return false;
	Texture cube_diffuse_texture{ cubeDiffuse, texture_diffuse_type, texture_diffuse_path }; 
	Texture cube_specular_texture{ cubeSpecular, texture_specular_type, texture_specular_path }; 

	cubeGeometry.m_Textures.push_back(cube_diffuse_texture); 
	cubeGeometry.m_Textures.push_back(cube_specular_texture); 
	cubeGeometry.m_IsLoaded = true; 
	CubeObject->m_Mesh->m_Geometry.push_back(cubeGeometry); 

	m_GameObjects.push_back(CubeObject);
	return true;
}



bool Scene::GetIsMeshLoaded(const std::string& Path, size_t& Index) const
{
	for (size_t i = 0; i < m_LoadedMeshes.size(); i++)
	{
		if (m_LoadedMeshes[i] -> GetPath() == Path)
		{
			Index = i; 
			return true;
		}
	}
	Index = -1;
	return false;
}

bool Scene::LoadShaders()
{
	Shader shader_light ("light");
	if (!shader_light.LoadShaderFromFile("src/shaders/vertex.glsl", "src/shaders/fragment.glsl"))
	{
		std::cerr << "Scene::LoadShaders() Error loading light shader" << std::endl; 
		return false;
	}

	m_Shaders.push_back(std::move(shader_light));
	Shader shader_muzzle_flash("muzzle_flash"); 
	if (!shader_muzzle_flash.LoadShaderFromFile("src/shaders/muzzle_flash_vertex.glsl", "src/shaders/muzzle_flash_fragment.glsl"))
	{
		std::cerr << "Scene::LoadShaders() Error loading muzzle flash shader"; 
		return false; 
	}
	m_Shaders.push_back(std::move(shader_muzzle_flash));
	Shader skybox("skybox"); 
	if (!skybox.LoadShaderFromFile("src/shaders/skybox_vertex.glsl", "src/shaders/skybox_fragment.glsl"))
	{
		std::cerr << "Scene::LoadShaders() Error loading skybox shader";
		return false;
	}
	
	m_Shaders.push_back(skybox);
	Shader Eagle("Eagle");
	if (!Eagle.LoadShaderFromFile("src/shaders/eagle_vertex.glsl", "src/shaders/eagle_fragment.glsl"))
	{
		std::cerr << "Scene::LoadShaders() Error loading skybox shader";
		return false;
	}
	m_Shaders.push_back(Eagle); 
	return true;
}

bool Scene::LoadEagle()
{
	std::vector <std::string> Suffixes = { "0.obj", "1.obj", "2.obj", "3.obj", "4.obj", "5.obj", "6.obj" }; 
	std::string BasePath = "Eagle"; 

	std::shared_ptr <Eagle> eaglePtr = std::make_shared<Eagle>();
	m_GameObjects.push_back(eaglePtr); 
	return eaglePtr->LoadFromFile(BasePath, Suffixes); 
}

void Scene::ProcessMouseClick(const glm::vec2 & Position, float dt)
{
	unsigned char ObjectID = 0; 
	glReadPixels((int)Position.x, (int)Position.y, 1,1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &ObjectID);

	if (ObjectID == CHEST_TOP_ID)
	{
		if (!ChestAnimationActive)
		{
			auto Chest = FindObjectByName("Chest_Top"); 
			if (Chest)
			{
				ChestAnimationActive = true;
				TargetChestRotation = ChestIsOpened ? ChestRotationOpened : ChestRotationClosed; 
				InitialChestRotation = Chest->GetWorldRotation();
				ChestAnimationStartTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f; 
			}
		}
	}

	if (ObjectID == REVOLVER_ID)
	{
		auto Revolver = FindObjectByName("Revolver"); 
		if (Revolver && !Revolver -> GetAttachParent())
		{
			Revolver->AttachToObject(GetActiveCamera().lock()); 
			Revolver->SetRelativeLocation({ -0.1f, -0.2f, 0.4f });
			glm::vec3 Rotation = glm::vec3(0.f, glm::radians(180.f), 0.f); 
			glm::quat Rotation_Quad(Rotation); 
			Revolver->SetRelativeRotation(Rotation_Quad);
		}
	}

	if (ObjectID == EAGLE_ID)
	{
		auto Revolver = FindObjectByName("Revolver"); 
		if (Revolver && Revolver->GetAttachParent() )
		{
			auto Eagle = FindObjectByName("Eagle"); 
			if (Eagle && Eagle->GetIsVisible())
			{
				MuzzleFlashStartTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;
				MuzzleFlashActive = true;
				Eagle->SetVisibility(false);
			}
				 
		}
	}
}

void Scene::ProcessInputAction(InputAction inputAction, float dt)
{
	auto ActiveCamera = GetActiveCamera().lock(); 
	if (!ActiveCamera || !ActiveCamera -> Movable )
		return; 
	glm::vec3 MovementDir; 
	switch (inputAction)
	{
		case ( InputAction::MoveForward ): 
		{
			MovementDir = ActiveCamera->GetFrontVector(); // camera is looking -Z
			break;
		}
		case (InputAction::MoveBackward): 
		{
			MovementDir = ActiveCamera->GetFrontVector() * -1.f;
			break;
		}
		case (InputAction::MoveLeft):
		{
			MovementDir = ActiveCamera->GetLeftVector(); // camera is looking -Z  
			break;
		}
		case (InputAction::MoveRight):
		{
			MovementDir = ActiveCamera->GetLeftVector() * -1.f;
			break;
		}

		case (InputAction::Fire):
		{
			break;
		}
		default:
		{

		}
	}
	glm::vec3 ResultLocation = ActiveCamera->GetWorldLocation() + MovementDir * dt * ActiveCamera->GetCameraSpeed();
	if (ResultLocation.x > 20 || ResultLocation.x < -20 || ResultLocation.y < 0.1 || ResultLocation.y > 35 || ResultLocation.z > 20 || ResultLocation.z < -20) // check if not out of bounds
		return;
	auto Eagle = FindObjectByName("Eagle");
	if (Eagle)
	{
		glm::vec3 EagleLocation = Eagle->GetWorldLocation();
		if (glm::length(EagleLocation - ActiveCamera->GetWorldLocation()) < 0.5) // check if not colliding with eagle
			return;
	}
	ActiveCamera->AddDeltaLocation(MovementDir * dt * ActiveCamera->GetCameraSpeed());
}

void Scene::ProcessMouseMovement(const glm::vec2& Offset, float dt)
{
	auto ActiveCamera = GetActiveCamera().lock();
	if (!ActiveCamera || !ActiveCamera->Movable)
		return;
	ActiveCamera->ProcessMouseMovement(Offset * dt);
}

std::weak_ptr<Camera> Scene::GetActiveCamera() const
{
	if (m_Cameras.size() <= m_ActiveCameraIndex)
	{
		std::cout << m_Cameras.size() << " " << m_ActiveCameraIndex << std::endl;
		return std::weak_ptr<Camera>();
	}
	return m_Cameras[m_ActiveCameraIndex];
}

Shader Scene::GetShaderByName(const std::string& ShaderName) const
{
	for (auto& Shader : m_Shaders)
	{
		if (Shader.GetShaderName() == ShaderName)
			return Shader;
	}
	return Shader();
}

std::shared_ptr<GameObject> Scene::FindObjectByName(const std::string& PartialName) const
{
	for (auto& Object : m_GameObjects)
	{
		if (Object->GetName().rfind(PartialName, 0) == 0)
			return Object;
	}
	return {};
}

void Scene::Render()
{
	if (GetActiveCamera().expired())
	{
		std::cerr << "Scene::Render() Error: Current active camera is not valid" << std::endl;
		return;
	}

	RenderSkybox();
	RenderEagle(); 
	auto Camera = GetActiveCamera().lock();

	if (MuzzleFlashActive)
	{
		RenderBillboard(FindObjectByName("muzzle_flash"));
	}


	glm::mat4 P = Camera->GetProjectionMatrix();
	glm::mat4 V = Camera->GetViewMatrix();
	Shader shader_light = GetShaderByName("light");
	shader_light.UseShader();
	SetSceneUniforms(shader_light);
	bool StencilOn = false;
	for (const auto& GameObject : m_GameObjects)
	{
		if (!GameObject->m_IsVisible)
			continue;
		if (GameObject->GetName().rfind("muzzle_flash", 0) == 0
			|| GameObject->GetName().rfind("skybox", 0) == 0) // skybox and muzzle flash are rendered in different pass 
		{
			continue;
		}

		if (GameObject->GetName().rfind("Revolver", 0) == 0)
		{

			 glEnable (GL_STENCIL_TEST);	
			 glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 
			 glStencilFunc(GL_ALWAYS, REVOLVER_ID, 0xff);
			 StencilOn = true; 
		}

		if (GameObject->GetName().rfind("Chest_Top", 0) == 0)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, CHEST_TOP_ID, 0xff);
			StencilOn = true;
		}

		if (GameObject->GetName().rfind("Eagle", 0) == 0)
		{
			continue; 
		}

		if (GameObject->GetName().rfind("Box", 0) == 0)
		{
			int a = 0;
		}

		glm::mat4 M = GameObject -> GetWorldModelMatrix();
		shader_light.SetMat4Parameter("PVMMatrix", P * V * M); 
		shader_light.SetMat4Parameter("MMatrix", M);

		if (GameObject->GetName().rfind("Water", 0) == 0)
		{
			glm::mat4 texTransform(1.0f);
			float timeAlpha = glm::abs ( glm::sin(glutGet(GLUT_ELAPSED_TIME) * 0.0001f) );
			glm::vec3 translate0{ 0.f, 0.f, 0.f }; 
			glm::vec3 translate1{ 1.f, 0.f, 0.f }; 
			glm::quat rotation0(glm::vec3(0.f, 0.f, 0.f));
			glm::quat rotation1( glm::vec3 (glm::radians(45.f), glm::radians(45.f), 0.f ));
			glm::vec3 lerpLocation = translate0 * (1.f - timeAlpha) + translate1 * (timeAlpha); 
			glm::quat slerpRotation = glm::slerp(rotation0, rotation1, timeAlpha); 

			texTransform = glm::toMat4(slerpRotation) * texTransform;
			texTransform = glm::translate(texTransform, lerpLocation );
			shader_light.SetMat4Parameter("WaterTransform", texTransform); 
			shader_light.SetBoolParameter("IsWater", true);

		}

		GameObject->Render(shader_light);
		shader_light.SetBoolParameter("IsWater", false);


		if (StencilOn)
		{
			glDisable(GL_STENCIL_TEST); 
			StencilOn = false; 
		}
	}
	CHECK_GL_ERROR();
	
}

void Scene::RenderSkybox()
{
	auto Camera = GetActiveCamera().lock();
	if (!Camera)
	{
		std::cerr << "Scene::RenderSkybox() Error: Invalid camera" << std::endl;
		return;
	}


	auto Skybox = FindObjectByName("skybox"); 
	if (!Skybox)
	{
		// std::cerr << "Scene::RenderSkybox() Error: Invalid skybox" << std::endl; 
		return; 
	}

	Shader SkyboxShader = GetShaderByName("skybox"); 
	glDisable(GL_DEPTH_TEST);
	glm::mat4 V = glm::mat4(glm::mat3(Camera->GetViewMatrix())); 
	glm::mat4 P = Camera->GetProjectionMatrix(); 
	SkyboxShader.UseShader();
	SkyboxShader.SetMat4Parameter("PVM", P * V);
	SkyboxShader.SetMat4Parameter("M", Skybox->GetWorldModelMatrix());
	SkyboxShader.SetMat4Parameter("V", Camera->GetViewMatrix());

	FogDayDefualts fd;
	FogNightDefaults fn;
	SkyboxShader.SetVec4Parameter("fogColor", isNight ? fn.color : fd.color);
	SkyboxShader.SetFloatParameter("fogMaxDistance", isNight ? fn.maxDistance : fd.maxDistance);
	SkyboxShader.SetFloatParameter("fogMinDistance", isNight ? fn.minDistance : fd.minDistance);


	glActiveTexture(GL_TEXTURE0);
	SkyboxShader.SetIntParameter("skyboxTexture", 0); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture); 
	Skybox->Render(); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); 
	glEnable(GL_DEPTH_TEST);
	CHECK_GL_ERROR(); 
	return; 
}

void Scene::RenderWater( std::shared_ptr <GameObject> & water ) const
{
}

void Scene::RenderEagle()
{

	auto Camera = GetActiveCamera().lock();
	auto Eagle = FindObjectByName("Eagle");

	if (!Camera || !Eagle)
		return; 

	if (!Eagle->GetIsVisible())
		return; 

	glm::mat4 V = Camera->GetViewMatrix();
	glm::mat4 P = Camera->GetProjectionMatrix();
	glm::mat4 M = Eagle->GetWorldModelMatrix(); 

	glm::mat4 PVM = P * V * M;

	// Set matrices parameters
	auto EagleShader = GetShaderByName("Eagle"); 
	EagleShader.UseShader();
	EagleShader.SetMat4Parameter("PVMMatrix", P * V * M);
	EagleShader.SetMat4Parameter("MMatrix", M);
	EagleShader.SetMat4Parameter("VMatrix", V);
	// Set fog parameters
	FogDayDefualts fd;
	FogNightDefaults fn;
	EagleShader.SetVec4Parameter("fogColor", isNight ? fn.color : fd.color);
	EagleShader.SetFloatParameter("fogMaxDistance", isNight ? fn.maxDistance : fd.maxDistance);
	EagleShader.SetFloatParameter("fogMinDistance", isNight ? fn.minDistance : fd.minDistance);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, EAGLE_ID, 0xff);
	Eagle->Render(EagleShader); 
	glDisable(GL_STENCIL_TEST);
}

void Scene::RenderBillboard(const std::shared_ptr<GameObject>& object) const
{
	if (!object)
	{
		std::cerr << "Scene::RenderBillboard() Error: invalid object" << std::endl; 
		return; 
	}
	auto Camera = GetActiveCamera().lock(); 

	Shader shader_muzzle_flash = GetShaderByName("muzzle_flash");
	shader_muzzle_flash.UseShader();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	const glm::mat4 V = Camera->GetViewMatrix(); 
	const glm::mat4 P = Camera->GetProjectionMatrix(); 

	// just take 3x3 rotation part of the view transform
	glm::mat4 billboardRotationMatrix = glm::mat4(
		V[0],
		V[1],
		V[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	// inverse view rotation
	billboardRotationMatrix = glm::rotate(billboardRotationMatrix, glm::radians(90.f), Camera->GetLeftVector()); // rotate plane to face camera
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	glm::mat4 matrix = glm::mat4(1.0f);
	
	matrix = glm::translate(matrix, object->GetWorldLocation());
	
	matrix = glm::scale(matrix, object->GetWorldTransform().Scale);

	matrix = matrix * billboardRotationMatrix; // make billboard to face the camera

	// Compute the final PVM matrix
	glm::mat4 PVM = P * V * matrix;

	//matrix = glm::mat4_cast(object->GetWorldRotation()) * matrix; 
	auto loc = object->GetWorldLocation();

	std::cout << "Muzzle flash location: " << loc.x << " , " << loc.y << " , " << loc.z << std::endl;

	shader_muzzle_flash.SetMat4Parameter("PVMMatrix", PVM);
	shader_muzzle_flash.SetIntParameter("frame", MuzzleFlashFrame);
	glActiveTexture(GL_TEXTURE0);
	shader_muzzle_flash.SetIntParameter("texSampler", 0);
	glBindTexture(GL_TEXTURE_2D, MuzzleFlashTexture);
	object->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	CHECK_GL_ERROR();
}
void Scene::SetSceneUniforms( const Shader & shader )
{

	auto ActiveCamera = GetActiveCamera().lock();
	if (!ActiveCamera)
	{
		std::cerr << "Scene::SetSceneUniforms() Error: not valid active camera" << std::endl; 
		return; 
	}
	shader.SetMat4Parameter("VMatrix", ActiveCamera -> GetViewMatrix() );
	shader.SetBoolParameter("isSpotlightActive", m_isSpotlightActive);
	shader.SetVec3Parameter("spotLight.direction", -ActiveCamera->GetFrontVector()); 
	// std::cout << "[" << ActiveCamera->GetFrontVector().x << " : "  << ActiveCamera->GetFrontVector().y << " : " << ActiveCamera->GetFrontVector().z << "]" << std::endl;
	shader.SetVec3Parameter("directionalLight.direction", m_DirectionalLightDirection);

	FogDayDefualts fd; 
	FogNightDefaults fn; 
	shader.SetVec4Parameter("fogColor", isNight ? fn.color : fd.color);
	shader.SetFloatParameter("fogMaxDistance", isNight ? fn.maxDistance : fd.maxDistance); 
	shader.SetFloatParameter("fogMinDistance", isNight ? fn.minDistance : fd.minDistance);


	for (size_t i = 0; i < m_PointLights.size(); i++)
	{
		std::string i_str = std::to_string(i); 
		shader.SetVec3Parameter("pointLights[" + i_str + "].position", m_PointLights[i].first);
		shader.SetBoolParameter("isPointLightActive[" + i_str + "]", m_PointLights[i].second);
	}


}
void Scene::Update( float dt )
{
	for (auto& object : m_GameObjects)
		object->Update(dt); 

	auto ActiveCamera = GetActiveCamera().lock(); 
	glm::vec3 Location = ActiveCamera->GetWorldTransform().Location;

	if (MuzzleFlashActive)
	{
		float ElapsedTime = (float)(glutGet(GLUT_ELAPSED_TIME) * 0.001f) - MuzzleFlashStartTime;
		const float MuzzleFlashLifetime = MuzzleFlashTotalFrames * MuzzleFlashFrameDuration; 
		if (ElapsedTime >= MuzzleFlashLifetime)
			MuzzleFlashActive = false;
		if ( MuzzleFlashActive )
		{
			MuzzleFlashFrame = (int)((ElapsedTime / MuzzleFlashLifetime) * MuzzleFlashTotalFrames); 
		}

	}
	if (ChestAnimationActive)
	{
		auto Chest = FindObjectByName("Chest_Top"); 
		if (Chest)
		{
			float ElapsedTime = (float)(glutGet(GLUT_ELAPSED_TIME) * 0.001f) - ChestAnimationStartTime;
			Chest->SetWorldRotation(glm::slerp(InitialChestRotation, TargetChestRotation, ElapsedTime));
			auto Rot = Chest->GetWorldRotation(); 
			if (ElapsedTime >= ChestAnimationTime)
			{
				ChestIsOpened = !ChestIsOpened; 
				ChestAnimationActive = false;
			}
		}
	}

	// UpdateTime 
	{
		float CurrentTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;
		float ElapsedTime = CurrentTime - DayStartTime;
		if (!isNight && ElapsedTime >= DayLength )
		{
			isNight = true;
			DayStartTime = CurrentTime; 

		}
		else if (isNight && ElapsedTime >= NightLength)
		{
			isNight = false; 
			DayStartTime = CurrentTime; 
		}
	}
}
void Scene::SetupCameras() // TODO: rework. Read cameras from scene file 
{
	glm::vec3 Camera0Location(3.0f, 1.0f, -5.0f);
	glm::vec3 Camera1Location(3.41f, 6.9f, 9.8f); 
	glm::vec3 Camera2Location(4.44f, 1.7f, -13.0f); 
	glm::vec3 CameraEagleLocation(0.0f, -0.2f, 0.0f);
	std::unique_ptr <Camera> Camera0 = std::make_unique <Camera>( "Camera0", Camera0Location, 0.0f, 0.f);
	std::unique_ptr <Camera> Camera1 = std::make_unique <Camera>( "Camera1", Camera1Location, 29.4f, -180.5f);
	std::unique_ptr <Camera> Camera2 = std::make_unique <Camera>( "Camera2", Camera2Location, 2.8f, -1.9f);
	std::unique_ptr <Camera> CameraEagle = std::make_unique<Camera>("CameraEagle", CameraEagleLocation, 45.f, 0.0f);
	auto Eagle = FindObjectByName("Eagle"); 
	if (Eagle)
	{
		CameraEagle->AttachToObject(Eagle); 
	}
	Camera1->Movable = false; 
	Camera2->Movable = false; 
	CameraEagle->Movable = false;


	m_Cameras.push_back(std::move(Camera0));
	m_Cameras.push_back(std::move(Camera1)); 
	m_Cameras.push_back(std::move(Camera2));
	m_Cameras.push_back(std::move(CameraEagle));
	m_ActiveCameraIndex = 0; 
	return; 
}

void Scene::ToggleSpotlight()
{
	m_isSpotlightActive = !m_isSpotlightActive; 
	std::cout << "Scene::TogleSpotlight() : " << m_isSpotlightActive << std::endl;
}
void Scene::SetCamerasAspectRation( float aspect )
{
	for (auto& Camera : m_Cameras)
	{
		Camera->SetProjectionParameters(45.f, aspect, 0.1f, 1500.f);
	}
}
void Scene::SetupLights()
{
	// Spotlight
	DirectionalLightDefaults params; 
	m_DirectionalLightDirection = params.direction;
	//TODO: point lights
	std::pair<glm::vec3, bool> pl = { {4.0f, 2.0f, 0.f}, true };
	std::pair<glm::vec3, bool> pl1 = { {3.41f, 6.9f, 9.8f}, true };
	//std::pair<glm::vec3, bool> pl2 = { {50.f, 50.f, 50.f}, true };
	//std::pair<glm::vec3, bool> pl3 = { {-50.f, 50.f, 50.f}, true };
	m_PointLights.push_back(pl); 
	m_PointLights.push_back(pl1);
	//m_PointLights.push_back(pl2);
	//m_PointLights.push_back(pl3);


	SetLightDefaultUniforms(); 
}

void Scene::SetLightDefaultUniforms()
{
	SpotlightDefaults slDefaults; 
	PointLightDefaults plDefaults;
	DirectionalLightDefaults dlDefaults; 

	Shader shader = GetShaderByName("light"); 
	shader.UseShader(); 
	// Directional light
	
	shader.SetVec3Parameter("directionalLight.direction", dlDefaults.direction);
	shader.SetVec3Parameter("directionalLight.ambientColor", dlDefaults.ambientColor);
	shader.SetVec3Parameter("directionalLight.diffuseColor", dlDefaults.diffuseColor);
	shader.SetVec3Parameter("directionalLight.specularColor", dlDefaults.specularColor);
	
	
	// Point lights 

	shader.SetIntParameter("nOfPointLights", m_PointLights.size()); 
	for (size_t i = 0; i < m_PointLights.size(); i ++ )
	{
		std::string i_str = std::to_string(i);
		shader.SetVec3Parameter("pointLights[" + i_str + "].ambientColor", plDefaults.ambientColor);
		shader.SetVec3Parameter("pointLights[" + i_str + "].diffuseColor", plDefaults.diffuseColor);
		shader.SetVec3Parameter("pointLights[" + i_str + "].specularColor", plDefaults.specularColor);
		shader.SetFloatParameter("pointLights[" + i_str + "].constant", plDefaults.constant);
		shader.SetFloatParameter("pointLights[" + i_str + "].linear", plDefaults.linear);
		shader.SetFloatParameter("pointLights[" + i_str + "].quadratic", plDefaults.quadratic);

	}

	// Spotlights
	shader.SetBoolParameter("isSpotlightActive", m_isSpotlightActive);
	shader.SetVec3Parameter("spotLight.ambientColor", slDefaults.ambientColor);
	shader.SetVec3Parameter("spotLight.diffuseColor", slDefaults.diffuseColor);
	shader.SetVec3Parameter("spotLight.specularColor", slDefaults.specularColor);

	shader.SetFloatParameter("spotLight.constant", slDefaults.constant);
	shader.SetFloatParameter("spotLight.linear", slDefaults.linear);
	shader.SetFloatParameter("spotLight.quadratic", slDefaults.quadratic);

	shader.SetFloatParameter("spotLight.cutoff", slDefaults.cutOff);
	shader.SetFloatParameter("spotLight.exponent", slDefaults.exponent);

	

}

void Scene::TogglePointLights()
{
	for (auto& pl : m_PointLights)
	{
		pl.second = !pl.second; 
	}
}

void Scene::ToggleCameraMovement()
{
	std::cout << "Scene::ToggleCameraMovement() called" << std::endl;
	auto cam = GetActiveCamera().lock(); 
	if (!cam)
		return; 
	cam->Movable = !cam->Movable; 
}
