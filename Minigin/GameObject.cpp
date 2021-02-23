#include "MiniginPCH.h"
#include "GameObject.h"
#include "Component.h"

engine::GameObject::~GameObject() = default;

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

void engine::GameObject::AddComponent(const std::weak_ptr<Component>& component)
{
	if (component.expired())
		std::cout << "Component is expired!\n";
	else if(component.use_count() != 1)
		std::cout << "Component is already added to another gameObject!\n";
	else
		m_pComponents.push_back(component.lock());
}