#pragma once
#include "Observer.h"

namespace engine
{
	class LivesComponent;
	class PlayerDeathObserver : public Observer
	{
	public:
		PlayerDeathObserver(std::weak_ptr<LivesComponent> target);
		
		void OnNotify(std::weak_ptr<GameObject> gameObject, Event event) override;

	private:
		std::weak_ptr<LivesComponent> m_pTarget = std::shared_ptr<LivesComponent>(nullptr);
	};
}

