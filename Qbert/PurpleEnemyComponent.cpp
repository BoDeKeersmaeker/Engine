#include "MiniginPCH.h"
#include "PurpleEnemyComponent.h"

#include "Audio.h"
#include "AudioLocator.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"
#include "Observer.h"
#include "SubjectComponent.h"

PurpleEnemyComponent::PurpleEnemyComponent(std::shared_ptr<engine::GameObject> owner, bool moveLeft, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown)
	:Component(owner)
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ moveCooldown }
	, m_MoveLeft{ moveLeft }
{
	if (moveLeft)
		if (!pStartNode.lock()->GetConnection(Direction::TOPLEFT).expired())
			m_pCurrentNode = pStartNode.lock()->GetConnection(Direction::TOPLEFT);
		else
			m_pCurrentNode = pStartNode;
	else
		if (!pStartNode.lock()->GetConnection(Direction::TOPRIGHT).expired())
			m_pCurrentNode = pStartNode.lock()->GetConnection(Direction::TOPRIGHT);
		else
			m_pCurrentNode = pStartNode;
	
	owner->AddComponent<engine::SubjectComponent>(std::make_shared<engine::SubjectComponent>(owner));
	m_pSubject = owner->GetComponent<engine::SubjectComponent>();
	
	if (!m_pCurrentNode.expired())
		m_pOwner.lock()->SetPosition(m_pCurrentNode.lock()->GetOwner().lock()->GetPosition());
}

void PurpleEnemyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= engine::EngineTime::GetInstance().GetElapsedSec();

	if (m_CurrentMoveCooldown <= 0)
	{
		int r = rand() % 2;
		if(m_MoveLeft)
		{
			if (r == 0)
				Move(Direction::TOPLEFT);
			else
				Move(Direction::LEFT);
		}
		else
		{
			if (r == 0)
				Move(Direction::TOPRIGHT);
			else
				Move(Direction::RIGHT);
		}
	}
}

void PurpleEnemyComponent::Render(const engine::Transform&)
{

}

std::weak_ptr<GridNodeComponent> PurpleEnemyComponent::GetCurrentNode() const
{
	return m_pCurrentNode;
}

bool PurpleEnemyComponent::CheckOverlap(std::weak_ptr<GridNodeComponent> node) const
{
	if(m_MoveLeft)
	{
		auto temp = m_pCurrentNode.lock()->GetConnection(Direction::BOTTOMRIGHT);
		return (!temp.expired() && temp.lock() == node.lock());
	}
	
	auto temp = m_pCurrentNode.lock()->GetConnection(Direction::BOTTOMLEFT);
	return (!temp.expired() && temp.lock() == node.lock());
}

void PurpleEnemyComponent::Move(Direction direction)
{
	m_CurrentMoveCooldown = m_MoveCooldown;

	engine::AudioLocator::getAudioSystem()->play(1);
	
	if (m_pCurrentNode.expired())
	{
		Die();
		return;
	}

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<Direction>(static_cast<size_t>(direction)));
	if (!temp.expired())
	{
		engine::DebugManager::GetInstance().print("Purple enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
	}
	else
		Die();
}

void PurpleEnemyComponent::Die() const
{
	m_pOwner.lock()->Destroy();
}