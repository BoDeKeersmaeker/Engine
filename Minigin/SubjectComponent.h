#pragma once
#include <vector>
#include "Component.h"

namespace engine
{
	class Observer;
	enum class Event;

	class SubjectComponent final : public Component
	{
	public:
		SubjectComponent(const std::shared_ptr<GameObject>& owner);
		void Update() override;
		void Render(const Transform& transform) override;
		
		void AddObserver(const std::shared_ptr<Observer>& observer);
		void RemoveObserver(const std::shared_ptr<Observer>& observer);
		
		void Notify(Event event, const std::weak_ptr<GameObject>& entity);
		void Notify(Event event, int number);
	
	private:
		std::vector<std::shared_ptr<Observer>> m_pObservers{};
	};
}