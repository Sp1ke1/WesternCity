#include "Mesh.h"


bool Mesh::LoadFromFile(const std::string & filename )
{
	Assimp::Importer importer;
	m_Path = m_ModelsFolder + filename;
	const aiScene* scene = importer.ReadFile( m_Path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "Mesh::LoadFromfile(): Assimp error during file read: " << importer.GetErrorString() << std::endl;
		return false;
	}
	
	if (!LoadGeometryFromAINode(scene->mRootNode, scene ))
	{
		std::cerr << "Mesh::LoadFromFile() :: Error loading mesh. Path: " + m_Path << std::endl; 
		return false; 
	}
	
    return true;
}

std::string Mesh::GetPath() const
{
	return m_Path;
}

void Mesh::Render(const Shader& shader)
{
	for (auto& MeshGeometry : m_Geometry)
	{
		MeshGeometry.Render(shader); 
	}
}

void Mesh::Render() const
{
	for (auto& MeshGeometry : m_Geometry)
		MeshGeometry.Render(); 
}

void Mesh::SetMaterial(const Material& material)
{
	m_Material = material; 
}

Material Mesh::GetMaterial() const
{
	return m_Material;
}

bool Mesh::LoadGeometryFromAINode( const aiNode* Node, const aiScene* Scene )
{
	for (unsigned int i = 0; i < Node ->mNumMeshes; i++)
	{
		aiMesh* _aiMesh = Scene ->mMeshes[Node ->mMeshes[i]];
		MeshGeometry meshGeometry ; 
		if (!meshGeometry.LoadFromAiMesh(_aiMesh, Scene))
		{
			// TODO: print error 
			continue; 
		}
		m_Geometry . emplace_back ( std::move ( meshGeometry ) );
	}

	for (unsigned int i = 0; i < Node -> mNumChildren; i++)
	{
		this -> LoadGeometryFromAINode (Node ->mChildren[i], Scene );
	}
	return true; 
}

const std::vector <MeshGeometry>& Mesh::GetMeshGeometry() const
{
	return m_Geometry;
}