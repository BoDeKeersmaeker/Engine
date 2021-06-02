#include "MiniginPCH.h"
#include "PurpleEnemyComponent.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"

engine::PurpleEnemyComponent::PurpleEnemyComponent(std::shared_ptr<GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown)
	:Component(owner)
	, m_pCurrentNode{ pStartNode }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
{
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
}

void engine::PurpleEnemyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= EngineTime::GetInstance().GetElapsedSec();

	if (m_CurrentMoveCooldown <= 0)
		Move(static_cast<MoveDirection>(rand() % 2 + 2));
}

void engine::PurpleEnemyComponent::Render(const Transform&)
{

}

void engine::PurpleEnemyComponent::Move(bool MoveLeft)
{
	if (m_pCurrentNode.expired())
		return;

	m_CurrentMoveCooldown = m_MoveCooldown;

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<ConnectionDirection>(static_cast<size_t>(direction)));
	if (!temp.expired())
	{
		DebugManager::GetInstance().print("Green enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
		m_pCurrentNode.lock()->Decrement();
	}
	else
		Die();
}

void engine::PurpleEnemyComponent::Die()
{
	m_pOwner.lock()->Destroy();
}