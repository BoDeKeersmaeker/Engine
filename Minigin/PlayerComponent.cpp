#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include "Observer.h"
#include "SubjectComponent.h"
#include "AudioLocator.h"
#include "Audio.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"

engine::PlayerComponent::PlayerComponent(std::shared_ptr<GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown, int lives)
	:Component(owner)
	, m_pStartNode{ pStartNode }
	, m_pCurrentNode{ pStartNode }
	, m_Lives{ lives }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
{
	owner->AddComponent<SubjectComponent>(std::make_shared<SubjectComponent>(owner));
	
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
	else
		//temp code for demo
		Respawn();
}

void engine::PlayerComponent::Update()
{
	if(m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= EngineTime::GetInstance().GetElapsedSec();
}

void engine::PlayerComponent::Render(const Transform&)
{
	
}

void engine::PlayerComponent::Move(Direction direction)
{
	if (m_CurrentMoveCooldown > 0 || m_pCurrentNode.expired())
		return;

	m_CurrentMoveCooldown = m_MoveCooldown;
	
	auto temp = m_pCurrentNode.lock()->GetConnection(direction);
	if (!temp.expired())
	{
		DebugManager::GetInstance().print("Qbert moved: " + std::to_string(static_cast<size_t>(direction)), PLAYER_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
		m_pCurrentNode.lock()->Increment();
	}
	else
		Die();
}

void engine::PlayerComponent::Die()
{
	m_Lives--;
	m_pOwner.lock()->GetComponent<SubjectComponent>().lock()->Notify(m_pOwner, Event::PlayerDied);

	AudioLocator::getAudioSystem()->play(0);
	
	Respawn();
	
	if (m_Lives  <= 0)
		m_pOwner.lock()->Destroy();
}

void engine::PlayerComponent::ChangeScore(int deltaScore)
{
	m_Score += deltaScore;
	m_pOwner.lock()->GetComponent<SubjectComponent>().lock()->Notify(m_pOwner, Event::ScoreChanged);
}

int engine::PlayerComponent::GetLives() const
{
	return m_Lives;
}

int engine::PlayerComponent::GetScore() const
{
	return m_Score;
}

void engine::PlayerComponent::Respawn()
{
	if (!m_pStartNode.expired())
	{
		m_pOwner.lock()->SetPosition(m_pStartNode.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = m_pStartNode;
	}
	else
	{
		//temp code for demo
		const auto x = static_cast<float>(rand() % 641);
		const auto y = static_cast<float>(rand() % 480);
		m_pOwner.lock()->SetPosition(x, y);
	}
}