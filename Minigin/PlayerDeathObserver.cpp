#include "MiniginPCH.h"
#include "PlayerDeathObserver.h"
#include "GameObject.h"
#include "CounterComponent.h"
#include "PlayerComponent.h"

engine::PlayerDeathObserver::PlayerDeathObserver(std::weak_ptr<CounterComponent> target)
	:Observer()
	,m_pTarget{ target }
{
	
}

void engine::PlayerDeathObserver::OnNotify(std::weak_ptr<GameObject> gameObject, Event event)
{
	switch (event)
	{
	case Event::PlayerDied:
		std::cout << "Player Died.\n";
		m_pTarget.lock()->SetCounter(gameObject.lock()->GetComponent<PlayerComponent>().lock()->GetLives());
		break;
	}
}
