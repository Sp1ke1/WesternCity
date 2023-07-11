#include "MeshGeometry.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::vector <Texture> MeshGeometry::LoadedTextures = {};


std::vector<Texture> MeshGeometry::GetTextureData() const
{
	return m_Textures; 
}

std::vector<Vertex> MeshGeometry::GetVerticisData() const
{
	return m_Vertices; 
}

std::vector <unsigned int> MeshGeometry::GetIndicisData() const
{
	return m_Indicis;
}

bool MeshGeometry::LoadFromAiMesh(const aiMesh* Mesh, const aiScene* Scene)
{
	if ( m_IsLoaded ) 
		return true;
	if (!LoadGeometryFromAiMesh ( Mesh ) )
	{
		return false;
	}

	if (!LoadMaterialsFromAiMesh(Mesh, Scene))
	{
		return false;
	}
	
	LoadGeometryToGPU(); 

	m_IsLoaded = true;
	return true;
}

bool MeshGeometry::GetIsTextureLoaded(const std::string & Path, size_t & Index )
{
	for (size_t i = 0; i < MeshGeometry::LoadedTextures.size(); i ++ )
	{
		if ( Path == MeshGeometry::LoadedTextures[i].Path )
		{
			Index = i; 
			return true;
		} 
	}
	Index = -1; 
	return false;
}

void MeshGeometry::Render(const Shader& shader) const
{
	
	BindTextures( shader ); 
	Render(); 
	UnbindTextures(); 
	
	
}
void MeshGeometry::BindTextures( const Shader & shader ) const
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string type = m_Textures[i].Type;
		if (type == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (type == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetIntParameter(("material." + type + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].Id);
		CHECK_GL_ERROR();
	}
	if (specularNr > 1)
	{
		shader.SetBoolParameter("material.hasSpecular", true);
	}
	else
		shader.SetBoolParameter("material.hasSpecular", false);

	CHECK_GL_ERROR();

}

void MeshGeometry::UnbindTextures() const
{
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	CHECK_GL_ERROR(); 

}

void MeshGeometry::Render() const
{
	glBindVertexArray(VAO); 
	if ( !m_Indicis.empty() )
		glDrawElements(GL_TRIANGLES, m_Indicis.size(), GL_UNSIGNED_INT, 0); 
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size()); 
	}
	glBindVertexArray(0); 
	CHECK_GL_ERROR();
}

bool MeshGeometry::LoadGeometryFromAiMesh( const aiMesh* Mesh )
{

	/* Load vertices */
	if (!Mesh)
	{
		std::cerr << "MeshGeometry::InitFromAiMesh => Invalid aiMesh" << std::endl;
		return false;
	}
	for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Location.x = Mesh->mVertices[i].x;
		vertex.Location.y = Mesh->mVertices[i].y;
		vertex.Location.z = Mesh->mVertices[i].z;

		vertex.Normal.x = Mesh->mNormals[i].x;
		vertex.Normal.y = Mesh->mNormals[i].y;
		vertex.Normal.z = Mesh->mNormals[i].z;

		if (Mesh->mTextureCoords[0])
		{
			vertex.TextureCoords.x = Mesh->mTextureCoords[0][i].x;
			vertex.TextureCoords.y = Mesh->mTextureCoords[0][i].y;

		}
		else
			vertex.TextureCoords = glm::vec2(0.0f);

		m_Vertices.push_back(vertex);
	}

	/* Load indecis  */
	for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
	{
		aiFace face = Mesh -> mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			m_Indicis.push_back(face.mIndices[j]);
	}

	
	return true;
}

bool MeshGeometry::LoadMaterialsFromAiMesh(const aiMesh* Mesh, const aiScene * Scene)
{
	if (Mesh->mMaterialIndex < 0)
		return true; 
	if (!Scene)
	{
		std::cerr << "MeshGeometry::LoadMaterialsFromAiMesh() => Mesh contains material indecis, but aiScene is invalid";
		return false;
	}
	aiMaterial* material = Scene ->mMaterials[Mesh->mMaterialIndex];
	if (!material)
	{
		std::cerr << "MeshGeometry::LoadMaterialsFromAiMesh() => Mesh contains material indecis, but aiMaterial is invalid" << std::endl; 
		return false;
	}

	LoadMaterialTexturesFromAiMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
	LoadMaterialTexturesFromAiMaterial(material, aiTextureType_SPECULAR, "texture_specular");
	return true;
}

void MeshGeometry::LoadMaterialTexturesFromAiMaterial(const aiMaterial* Material, aiTextureType TextureType, const std::string& TypeName)
{
	if (!Material)
		return; 
	unsigned int TextureCount = Material->GetTextureCount(TextureType); 
	for (unsigned int i = 0; i < TextureCount; i++)
	{
		size_t LoadedIndex; 
		aiString str;
		Material ->GetTexture(TextureType, i, &str);
		Texture texture;
		std::string TexturePath = m_TexturesFolder + std::string(str.C_Str()); 
		if (GetIsTextureLoaded(TexturePath, LoadedIndex) )
		{
			texture = MeshGeometry::LoadedTextures[i]; 
			m_Textures.push_back(texture); 
			continue; 
		}
		texture.Id = TextureFromFile(TexturePath.c_str()); // TODO: rewrite texture from file func 
		texture.Type = TypeName;
		texture.Path = str.C_Str();
		m_Textures.push_back ( texture );
	}
}

void MeshGeometry::LoadGeometryToGPU()
{
	if (m_Vertices.empty())
		return;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);


	if (!m_Indicis.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicis.size() * sizeof(unsigned int), &m_Indicis[0], GL_STATIC_DRAW);
	}


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));

	glBindVertexArray(0);

	CHECK_GL_ERROR(); 

}
