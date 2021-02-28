#pragma once
#include "Observer.h"

namespace engine
{
	class CounterComponent;
	class PlayerDeathObserver : public Observer
	{
	public:
		PlayerDeathObserver(std::weak_ptr<CounterComponent> target);
		
		void OnNotify(std::weak_ptr<GameObject> gameObject, Event event) override;

	private:
		std::weak_ptr<CounterComponent> m_pTarget = std::shared_ptr<CounterComponent>(nullptr);
	};
}

