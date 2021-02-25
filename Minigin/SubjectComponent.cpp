#include "MiniginPCH.h"
#include "SubjectComponent.h"
#include "Observer.h"

engine::SubjectComponent::SubjectComponent(std::shared_ptr<GameObject> owner)
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

void engine::SubjectComponent::RemoveObserver(const std::shared_ptr<Observer>&)
{
	
}

void engine::SubjectComponent::Notify(std::weak_ptr<GameObject> entity, Event event)
{
	for (int i{ 0 }; i < m_pObservers.size(); ++i)
	{
		m_pObservers[i]->OnNotify(entity, event);
	}
}
