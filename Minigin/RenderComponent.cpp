#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

dae::RenderComponent::RenderComponent()
	:Component(ComponentType::invalid)
{

}

void dae::RenderComponent::Update()
{
}

void dae::RenderComponent::Render(const Transform& transform)
{
	const auto pos = transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}
