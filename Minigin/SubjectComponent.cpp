#include "MiniginPCH.h"
#include "SubjectComponent.h"
#include "Observer.h"

engine::SubjectComponent::SubjectComponent(const std::shared_ptr<GameObject>& owner)
	:Component(owner)
{
	
}

void engine::SubjectComponent::Update()
{
	
}

void engine::SubjectComponent::Render(const Transform&)
{
	
}

void engine::SubjectComponent::AddObserver(const std::shared_ptr<Observer>& observer)
{
	m_pObservers.push_back(observer);
}

void engine::SubjectComponent::RemoveObserver(const std::shared_ptr<Observer>& observer)
{
	m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), observer), m_pObservers.end());
}

void engine::SubjectComponent::Notify(Event event, const std::weak_ptr<GameObject>& entity)
{
	for (size_t i{ 0 }; i < m_pObservers.size(); ++i)
		m_pObservers[i]->OnNotify(event, entity);
}

void engine::SubjectComponent::Notify(Event event, int number)
{
	for (size_t i{ 0 }; i < m_pObservers.size(); ++i)
		m_pObservers[i]->OnNotify(event, number);
}