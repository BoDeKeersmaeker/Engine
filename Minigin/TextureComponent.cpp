#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "RenderComponent.h"
#include "Transform.h"

dae::TextureComponent::TextureComponent()
	:Component(ComponentType::Texture)
	,m_RenderComp(std::make_shared<RenderComponent>())
{

}

void dae::TextureComponent::Update()
{

}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_RenderComp->SetTexture(filename);
}

void dae::TextureComponent::Render(const Transform& transform)
{
	m_RenderComp->Render(transform);
}
