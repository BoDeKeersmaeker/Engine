#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "RenderComponent.h"
#include "Transform.h"

engine::TextureComponent::TextureComponent()
	:Component(ComponentType::Texture)
	,m_RenderComp(std::make_shared<RenderComponent>())
{

}

void engine::TextureComponent::Update()
{

}

void engine::TextureComponent::SetTexture(const std::string& filename)
{
	m_RenderComp->SetTexture(filename);
}

void engine::TextureComponent::Render(const Transform& transform)
{
	m_RenderComp->Render(transform);
}
