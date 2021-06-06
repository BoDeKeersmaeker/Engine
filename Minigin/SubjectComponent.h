#pragma once
#include <vector>
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
		
		virtual void AddObserver(std::shared_ptr<Observer> observer);
		virtual void RemoveObserver(std::shared_ptr<Observer> observer);
		
		void Notify(Event event, std::weak_ptr<GameObject> entity);
		void Notify(Event event, int number);
	
	private:
		std::vector<std::shared_ptr<Observer>> m_pObservers;
	};
}