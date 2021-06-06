#include "MiniginPCH.h"
#include "PurpleEnemyComponent.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"
#include "Observer.h"
#include "SubjectComponent.h"

PurpleEnemyComponent::PurpleEnemyComponent(std::shared_ptr<engine::GameObject> owner, bool moveLeft, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown)
	:Component(owner)
	, m_pCurrentNode{ pStartNode }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
	, m_MoveLeft{ moveLeft }

{
	owner->AddComponent<engine::SubjectComponent>(std::make_shared<engine::SubjectComponent>(owner));
	m_pSubject = owner->GetComponent<engine::SubjectComponent>();
	
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
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
				Move(engine::Direction::TOPLEFT);
			else
				Move(engine::Direction::LEFT);
		}
		else
		{
			if (r == 0)
				Move(engine::Direction::TOPRIGHT);
			else
				Move(engine::Direction::RIGHT);
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

bool PurpleEnemyComponent::CheckOverlap(std::weak_ptr<GridNodeComponent> node)
{
	if(m_MoveLeft)
	{
		auto temp = m_pCurrentNode.lock()->GetConnection(engine::Direction::BOTTOMRIGHT);
		return (!temp.expired() && temp.lock() == node.lock());
	}
	
	auto temp = m_pCurrentNode.lock()->GetConnection(engine::Direction::BOTTOMLEFT);
	return (!temp.expired() && temp.lock() == node.lock());
}

void PurpleEnemyComponent::Move(engine::Direction direction)
{
	m_CurrentMoveCooldown = m_MoveCooldown;
	
	if (m_pCurrentNode.expired())
	{
		Die();
		return;
	}

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<engine::Direction>(static_cast<size_t>(direction)));
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