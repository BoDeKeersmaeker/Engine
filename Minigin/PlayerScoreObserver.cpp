#include "MiniginPCH.h"
#include "PlayerScoreObserver.h"
#include "GameObject.h"
#include "CounterComponent.h"
#include "PlayerComponent.h"

engine::PlayerScoreObserver::PlayerScoreObserver(std::weak_ptr<CounterComponent> target)
	:Observer()
	, m_pTarget{ target }
{

}

void engine::PlayerScoreObserver::OnNotify(std::weak_ptr<GameObject> gameObject, Event event)
{
	switch (event)
	{
	case Event::ScoreChanged:
		std::cout << "ScoreAdded.\n";
		m_pTarget.lock()->SetCounter(gameObject.lock()->GetComponent<PlayerComponent>().lock()->GetScore());
		break;
	}
}