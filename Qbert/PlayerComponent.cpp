#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include "Observer.h"
#include "SubjectComponent.h"
#include "AudioLocator.h"
#include "Audio.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "DiscComponent.h"
#include "EngineTime.h"

PlayerComponent::PlayerComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown, int lives)
	:Component(owner)
	, m_pStartNode{ pStartNode }
	, m_pCurrentNode{ pStartNode }
	, m_Lives{ lives }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
{
	owner->AddComponent<engine::SubjectComponent>(std::make_shared<engine::SubjectComponent>(owner));
	m_pSubject = owner->GetComponent<engine::SubjectComponent>();
	
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
	else
		//temp code for demo
		Respawn();
}

void PlayerComponent::Update()
{
	if(m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= engine::EngineTime::GetInstance().GetElapsedSec();
}

void PlayerComponent::Render(const engine::Transform&)
{
	
}

void PlayerComponent::Move(engine::Direction direction)
{
	if (m_IsOnDisk || m_CurrentMoveCooldown > 0 || m_pCurrentNode.expired())
		return;

	m_CurrentMoveCooldown = m_MoveCooldown;
	
	auto temp = m_pCurrentNode.lock()->GetConnection(direction);
	if (!temp.expired())
	{
		engine::DebugManager::GetInstance().print("Qbert moved: " + std::to_string(static_cast<size_t>(direction)), PLAYER_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
		m_pCurrentNode.lock()->Increment();
	}
	else
	{
		std::weak_ptr<DiscComponent> disc;
		switch (direction)
		{
		case engine::Direction::TOPLEFT:
			disc = m_pCurrentNode.lock()->GetDiscs().first;
			if(!disc.expired())
				disc.lock()->Activate(m_pOwner.lock()->GetComponent<PlayerComponent>());
			else
				Die();
			break;
		case engine::Direction::TOPRIGHT:
			disc = m_pCurrentNode.lock()->GetDiscs().first;
			if (!disc.expired())
				disc.lock()->Activate(m_pOwner.lock()->GetComponent<PlayerComponent>());
			else
				Die();
			break;
		default:
			Die();
			break;
		}
	}
}

void PlayerComponent::Die()
{
	m_Lives--;
	if (!m_pSubject.expired())
		m_pSubject.lock()->Notify(m_pOwner, engine::Event::PlayerDied);

	engine::AudioLocator::getAudioSystem()->play(0);
	
	Respawn();
	
	if (m_Lives  <= 0)
		m_pOwner.lock()->Destroy();
}

void PlayerComponent::SetCurrentNode(std::weak_ptr<GridNodeComponent> pNode)
{
	m_pCurrentNode = pNode;
}

void PlayerComponent::SetIsOnDisk(bool isOnDisk)
{
	m_IsOnDisk = isOnDisk;
}

std::weak_ptr<GridNodeComponent> PlayerComponent::GetCurrentNode()
{
	return m_pCurrentNode;
}

bool PlayerComponent::IsOnDisk() const
{
	return m_IsOnDisk;
}

int PlayerComponent::GetLives() const
{
	return m_Lives;
}

void PlayerComponent::Reset(std::weak_ptr<GridNodeComponent> newStartNode)
{
	if (!newStartNode.expired())
		m_pStartNode = newStartNode;
	m_pCurrentNode = m_pStartNode;
	m_pOwner.lock()->SetPosition(m_pStartNode.lock()->GetOwner().lock()->GetPosition());
}

void PlayerComponent::Respawn()
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