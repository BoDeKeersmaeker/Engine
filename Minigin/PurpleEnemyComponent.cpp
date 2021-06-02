#include "MiniginPCH.h"
#include "PurpleEnemyComponent.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"

engine::PurpleEnemyComponent::PurpleEnemyComponent(std::shared_ptr<GameObject> owner, bool moveLeft, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown)
	:Component(owner)
	, m_pCurrentNode{ pStartNode }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
	, m_MoveLeft{ moveLeft }

{
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
}

void engine::PurpleEnemyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= EngineTime::GetInstance().GetElapsedSec();

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

void engine::PurpleEnemyComponent::Render(const Transform&)
{

}

void engine::PurpleEnemyComponent::Move(Direction direction)
{
	m_CurrentMoveCooldown = m_MoveCooldown;
	
	if (m_pCurrentNode.expired())
		return;

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<Direction>(static_cast<size_t>(direction)));
	if (!temp.expired())
	{
		DebugManager::GetInstance().print("Purple enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
	}
	else
		Die();
}

void engine::PurpleEnemyComponent::Die() const
{
	m_pOwner.lock()->Destroy();
}