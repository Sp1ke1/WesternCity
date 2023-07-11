#include "GameObject.h"

GameObject::GameObject(const std::string& Name, const std::shared_ptr<Mesh>& mesh, const Transform& transform)
	: SceneObject ( Name, transform ),
	  m_Mesh(mesh)
{


}

void GameObject::Render(const Shader& shader)
{
	if (m_Mesh.get())
	{
		shader.BindMaterial(m_Mesh->GetMaterial()); 
		m_Mesh->Render(shader); 
	}
}
void GameObject::Render()
{
	if (m_Mesh.get())
	{
		m_Mesh->Render(); 
	}
}

void GameObject::SetVisibility(bool isVisible)
{
	m_IsVisible = isVisible; 
}

bool GameObject::GetIsVisible() const
{
	return m_IsVisible;
}


