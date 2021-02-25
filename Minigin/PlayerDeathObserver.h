#pragma once
#include "Observer.h"

namespace engine
{
	class GameObject;
	
	class PlayerDeathObserver : public Observer
	{
	public:
		PlayerDeathObserver(std::weak_ptr<GameObject> target);
		
		void OnNotify(std::weak_ptr<GameObject> gameObject, Event event) override;

	private:
		std::weak_ptr<GameObject> m_pTarget = std::shared_ptr<GameObject>(nullptr);
	};
}

