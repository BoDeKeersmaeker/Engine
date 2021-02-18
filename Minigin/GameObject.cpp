#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (std::shared_ptr<Component> component : m_pComponents)
		component->Update();
}

void dae::GameObject::Render() const
{
	
	for (std::shared_ptr<Component> component : m_pComponents)
		component->Render(m_Transform);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_pComponents.push_back(component);
}