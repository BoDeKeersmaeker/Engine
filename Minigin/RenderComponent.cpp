#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

engine::RenderComponent::RenderComponent(const std::shared_ptr<GameObject>& owner, const std::string& path, Float2 offset)
	:Component(owner)
	, m_Offset{ offset }
{
	SetTexture(path);
}

void engine::RenderComponent::Update()
{
	
}

void engine::RenderComponent::Render(const Transform& transform)
{
	const auto pos = transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x + m_Offset.x, pos.y + m_Offset.y);
}

void engine::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = engine::ResourceManager::GetInstance().LoadTexture(filename);
}
