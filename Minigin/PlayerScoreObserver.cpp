#include "MiniginPCH.h"
#include "PlayerScoreObserver.h"
#include "GameObject.h"
#include "ScoreComponent.h"
#include "PlayerComponent.h"

engine::PlayerScoreObserver::PlayerScoreObserver(std::weak_ptr<ScoreComponent> target)
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
		m_pTarget.lock()->SetScore(gameObject.lock()->GetComponent<PlayerComponent>().lock()->GetScore());
		break;
	}
}