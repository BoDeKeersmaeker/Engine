#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

engine::RenderComponent::RenderComponent(std::shared_ptr<GameObject> owner)
	:Component(owner)
{

}

void engine::RenderComponent::Update()
{
}

void engine::RenderComponent::Render(const Transform& transform)
{
	const auto pos = transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void engine::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = engine::ResourceManager::GetInstance().LoadTexture(filename);
}
