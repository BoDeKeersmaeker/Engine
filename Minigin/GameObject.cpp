#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (std::pair< ComponentType, std::shared_ptr<Component>> component : m_pComponents)
		component.second->Update();
}

void dae::GameObject::Render() const
{
	
	for (std::pair< ComponentType, std::shared_ptr<Component>> component : m_pComponents)
		component.second->Render(m_Transform);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	m_pComponents[component->GetType()] = component;
}

std::shared_ptr<dae::Component> dae::GameObject::GetComponent(ComponentType type)
{
	return m_pComponents[type];
}