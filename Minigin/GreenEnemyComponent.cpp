#include "MiniginPCH.h"
#include "GreenEnemyComponent.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"

engine::GreenEnemyComponent::GreenEnemyComponent(std::shared_ptr<GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown)
	:Component(owner)
	, m_pCurrentNode{ pStartNode }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
{
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
}

void engine::GreenEnemyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= EngineTime::GetInstance().GetElapsedSec();

	if(m_CurrentMoveCooldown <= 0)
		Move(static_cast<Direction>(rand() % 2 + 2));
}

void engine::GreenEnemyComponent::Render(const Transform&)
{

}

void engine::GreenEnemyComponent::Move(Direction direction)
{
	m_CurrentMoveCooldown = m_MoveCooldown;
	
	if (m_pCurrentNode.expired())
		return;

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<Direction>(static_cast<size_t>(direction)));
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

void engine::GreenEnemyComponent::Die()
{
	m_pOwner.lock()->Destroy();
}