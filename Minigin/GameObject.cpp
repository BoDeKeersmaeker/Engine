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

void engine::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}