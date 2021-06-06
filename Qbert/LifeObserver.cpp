#include "MiniginPCH.h"
#include "LifeObserver.h"
#include "GameObject.h"
#include "CounterComponent.h"
#include "GameManagerComponent.h"
#include "PlayerComponent.h"

LifeObserver::LifeObserver(std::weak_ptr<engine::CounterComponent> pTarget, std::weak_ptr<engine::GameObject> pManager)
	:Observer()
	, m_pTarget{ pTarget }
	, m_pManager{ pManager }
{

}

void LifeObserver::OnNotify(engine::Event event, std::weak_ptr<engine::GameObject> object)
{
	auto comp = object.lock()->GetComponent<PlayerComponent>();
	if (!m_pTarget.expired() && !comp.expired() && event == engine::Event::PlayerDied)
	{
		engine::DebugManager::GetInstance().print("Lives changed.", OBSERVER_DEBUG);
		m_pTarget.lock()->SetCounter(comp.lock()->GetLives());

		if (!m_pManager.expired())
		{
			auto managerComp = m_pManager.lock()->GetComponent<GameManagerComponent>();
			if (!managerComp.expired())
				managerComp.lock()->ClearEnemies();
		}
	}
}

void LifeObserver::OnNotify(engine::Event, int)
{
	
}