#include "MiniginPCH.h"
#include "ScoreObserver.h"
#include "GameObject.h"
#include "CounterComponent.h"

ScoreObserver::ScoreObserver(const std::weak_ptr<engine::CounterComponent>& target)
	:Observer()
	, m_pTarget{ target }
{

}

void ScoreObserver::OnNotify(engine::Event, const std::weak_ptr<engine::GameObject>&)
{
	
}

void ScoreObserver::OnNotify(engine::Event event, int number)
{
	if (!m_pTarget.expired() && event == engine::Event::ScoreChanged)
	{
		engine::DebugManager::GetInstance().print("ScoreAdded.", OBSERVER_DEBUG);
		m_score += number;
		m_pTarget.lock()->SetCounter(m_score);
	}
}