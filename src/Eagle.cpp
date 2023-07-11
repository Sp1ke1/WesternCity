#include "Eagle.h"

void Eagle::Update(float dt)
{
	Transform t = GetWorldTransform();
	EagleAngle += EagleSpeed * dt;
	if (EagleAngle >= 360)
		EagleAngle = 0;
	glm::vec3 newPosition = EagleCircleCenter + glm::vec3(
		EagleCircleRadius * glm::cos(glm::radians(EagleAngle)),
		0.0f,
		EagleCircleRadius * glm::sin(glm::radians(EagleAngle))
	);
	// for circle tanget is perpendicular to radius at given point. Note that perpendicular vector to [x,y] is [-y,x] 
	glm::vec3 tangent = glm::normalize(glm::vec3(-t.Location.z, 0.f, t.Location.x));
	SetWorldRotation(glm::rotation(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)), tangent));
	SetWorldLocation(newPosition);


	float currentTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f; 
	float elapsedTime = currentTime - FrameStartTime; 
	if (elapsedTime >= FrameLength)
	{
		SwitchAnimationFrame();
		CurrentAnimationAlpha = 0.f; 
		FrameStartTime = currentTime; 
		return; 
	}

	CurrentAnimationAlpha = elapsedTime / FrameLength; 
}

void Eagle::SwitchAnimationFrame()
{
	if (NextAnimationFrame + 1 >= m_Meshes.size() )
		NextAnimationFrame = 0; 
	else
		NextAnimationFrame++;
	if (CurrentAnimationFrame + 1 >= m_Meshes.size() )
		CurrentAnimationFrame = 0;
	else 
		CurrentAnimationFrame++; 
}

bool Eagle::LoadFromFile(const std::string& baseName, const std::vector<std::string>& suffixes)
{
	SetName("Eagle"); 
	Transform EagleTransform = { {0.f, 5.f, 0.f}, {0.f, 0.f, 0.f}, {0.2f, 0.2f, 0.2f } };
	SetWorldTransform(EagleTransform);
	for (auto& suff : suffixes)
	{
		std::shared_ptr <Mesh> frameMesh = std::make_shared<Mesh>();
		if (!frameMesh->LoadFromFile(baseName + suff))
		{
			std::cerr << "Eagle::LoadFromFile() Error => can't load animation frame with given path: " << baseName + suff << std::endl; 
			return false;
		}
		m_Meshes.push_back(frameMesh); 
	}
	if (m_Meshes.empty())
	{
		std::cerr << "Eagle::LoadFromFile() Error => can't load single mesh, probably suffixes are empty" << std::endl;
		return false;
	}
	if (!*m_Meshes.begin())
	{
		std::cerr << "Eagle::LoadFromFile Error => invalid mesh" << std::endl; 
		return false;
	}
	m_EagleTextures = m_Meshes.begin()->get()->GetMeshGeometry().begin()->GetTextureData();
	nOfVertices = m_Meshes.begin()->get()->GetMeshGeometry().begin()->GetVerticisData().size();
	std::vector <unsigned int> Indicis = m_Meshes.begin()->get()->GetMeshGeometry().begin()->GetIndicisData();
	nOfFaces = Indicis.size();
	std::vector <Vertex> Verticis; 


	for (auto Mesh : m_Meshes)
	{
		if (Mesh)
		{
			auto VertexData = Mesh->GetMeshGeometry().begin()->GetVerticisData();
			Verticis.insert( Verticis.end(), VertexData.begin(), VertexData.end() ); 
		}
	}

	glGenVertexArrays(1, &m_EagleVAO);
	glGenBuffers(1, &m_EagleEBO);
	glGenBuffers(1, &m_EagleVBO);
	glBindVertexArray(m_EagleVAO);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, m_EagleVBO);
	glBufferData(GL_ARRAY_BUFFER, Verticis.size() * sizeof ( Vertex ), &Verticis[0], GL_STATIC_DRAW);
	// generate and initialize the element buffer object -> variable resources.ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EagleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indicis.size() * sizeof ( unsigned int ), &Indicis[0], GL_STATIC_DRAW);
	// enable and initialize the position attribute array
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//glEnableVertexAttribArray(5);
	//glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
	CHECK_GL_ERROR();


	/*glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
	CHECK_GL_ERROR(); 
		
	return true;
}


void Eagle::Render(const Shader& shader)
{
	CHECK_GL_ERROR();
	if (CurrentAnimationFrame >= m_Meshes.size())
	{
		std::cerr << "Eagle::Render() Error => CurrentAnimationFrame is bigger that mesh array size" << std::endl; 
		return; 
	}

	if (NextAnimationFrame >= m_Meshes.size())
	{
		std::cerr << "Eagle::Render() Error => NextAnimationFrame is bigger than mesh array size" << std::endl; 
		return; 
	}
	//std::cout << "Current Frame:" << CurrentAnimationFrame << std::endl; 
	//std::cout << "Next Frame:" << NextAnimationFrame << std::endl; 
	//std::cout << "Animation Alpha: " << CurrentAnimationAlpha << std::endl; 
	shader.SetFloatParameter("AnimationAlpha", CurrentAnimationAlpha);

	// update animation geometry
	glBindVertexArray(m_EagleVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(CurrentAnimationFrame * sizeof(Vertex) * nOfVertices + 0) );
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NextAnimationFrame * sizeof(Vertex) * nOfVertices + 0));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TextureCoords)));

	//std::cout << "Offset1: " << CurrentAnimationFrame * sizeof(Vertex) * nOfVertices + 0 << std::endl;
	//std::cout << "Offset2: " << CurrentAnimationFrame * sizeof(Vertex) * nOfVertices + offsetof(Vertex, Normal) << std::endl;
	//std::cout << "Offset3: " << CurrentAnimationFrame * sizeof(Vertex) * nOfVertices + offsetof(Vertex, TextureCoords) << std::endl;
	//std::cout << "Location: " << GetWorldLocation().x << " , " << GetWorldLocation().y << " , " << GetWorldLocation().z << std::endl; 

	//glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NextAnimationFrame * sizeof(Vertex) * nOfVertices + offsetof(Vertex, TextureCoords)));
	CHECK_GL_ERROR();
	

	// bind texture
	glActiveTexture(GL_TEXTURE0);
	CHECK_GL_ERROR();
	shader.SetIntParameter("texture_diffuse1", 0);
	glBindTexture(GL_TEXTURE_2D, m_EagleTextures.begin()->Id);
	CHECK_GL_ERROR();

	// draw
	glDrawElements(GL_TRIANGLES, nOfFaces, GL_UNSIGNED_INT, (void*)0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
}