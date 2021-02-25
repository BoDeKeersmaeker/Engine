#include "MiniginPCH.h"
#include "SceneObject.h"
#include "Observer.h"

void engine::SceneObject::AddObserver(const std::shared_ptr<Observer>& observer)
{
	m_pObservers.push_back(observer);
}

void engine::SceneObject::RemoveObserver(const std::shared_ptr<Observer>&)
{

}

void engine::SceneObject::Notify(std::weak_ptr<GameObject> entity, Event event)
{
	for (int i{ 0 }; i < m_pObservers.size(); ++i)
	{
		m_pObservers[i]->OnNotify(entity, event);
	}
}