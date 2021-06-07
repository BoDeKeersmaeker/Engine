#include "MiniginPCH.h"
#include "GameObject.h"
#include "Component.h"

void engine::GameObject::Update()
{
	for (auto& temp : m_pComponents)
			temp->Update();
}

void engine::GameObject::Render() const
{
	for (const auto& temp : m_pComponents)
		temp->Render(m_Transform);
}

engine::Float2 engine::GameObject::GetPosition() const
{
	return { m_Transform.GetPosition().x, m_Transform.GetPosition().y };
}

void engine::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void engine::GameObject::SetPosition(const Float2& pox)
{
	m_Transform.SetPosition(pox.x, pox.y, 0.0f);
}

const engine::Transform& engine::GameObject::GetTransform() const
{
	return m_Transform;
}