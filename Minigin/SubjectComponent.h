#pragma once
#include "Component.h"
namespace engine
{
	class Observer;
	enum class Event;

	class SubjectComponent : public Component
	{
	public:
		SubjectComponent(std::shared_ptr<GameObject> owner);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;
		
		virtual void AddObserver(const std::shared_ptr<Observer>& observer);
		virtual void RemoveObserver(const std::shared_ptr<Observer>& observer);
		
		void Notify(std::weak_ptr<GameObject> entity, Event event);
	
	private:
		std::vector<std::shared_ptr<Observer>> m_pObservers;
	};
}