#include "MiniginPCH.h"
#include "GameObject.h"
#include "Component.h"
#include "Observer.h"

engine::GameObject::~GameObject()
{
	
}

void engine::GameObject::Update()
{
	for (std::shared_ptr<Component> component : m_pComponents)
		component->Update();
}

void engine::GameObject::Render() const
{
	
	for (std::shared_ptr<Component> component : m_pComponents)
		component->Render(m_Transform);
}

engine::Float2 engine::GameObject::GetPosition()
{
	return { m_Transform.GetPosition().x, m_Transform.GetPosition().y };
}

void engine::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void engine::GameObject::SetPosition(Float2 pox)
{
	m_Transform.SetPosition(pox.x, pox.y, 0.0f);
}

const engine::Transform& engine::GameObject::GetTransform()
{
	return m_Transform;
}
