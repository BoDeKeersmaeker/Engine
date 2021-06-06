#include "MiniginPCH.h"
#include "SubjectComponent.h"
#include "Observer.h"

engine::SubjectComponent::SubjectComponent(std::shared_ptr<GameObject> owner)
	:Component(owner)
	, m_pObservers{ }
{
	
}

void engine::SubjectComponent::Update()
{
	
}

void engine::SubjectComponent::Render(const Transform&)
{
	
}

void engine::SubjectComponent::AddObserver(std::shared_ptr<Observer> observer)
{
	m_pObservers.push_back(observer);
}

void engine::SubjectComponent::RemoveObserver(std::shared_ptr<Observer>)
{
	
}

void engine::SubjectComponent::Notify(Event event, std::weak_ptr<GameObject> entity)
{
	for (size_t i{ 0 }; i < m_pObservers.size(); ++i)
		m_pObservers[i]->OnNotify(event, entity);
}

void engine::SubjectComponent::Notify(Event event, int number)
{
	for (size_t i{ 0 }; i < m_pObservers.size(); ++i)
		m_pObservers[i]->OnNotify(event, number);
}