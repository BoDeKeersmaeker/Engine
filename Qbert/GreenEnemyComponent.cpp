#include "MiniginPCH.h"
#include "GreenEnemyComponent.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"

GreenEnemyComponent::GreenEnemyComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode, float moveCooldown)
	:Component(owner)
	, m_pCurrentNode{ pStartNode }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ 0.f }
{
	if (!pStartNode.expired())
		m_pOwner.lock()->SetPosition(pStartNode.lock()->GetOwner().lock()->GetPosition());
}

void GreenEnemyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= engine::EngineTime::GetInstance().GetElapsedSec();

	if(m_CurrentMoveCooldown <= 0)
		Move(static_cast<engine::Direction>(rand() % 2 + 2));
}

void GreenEnemyComponent::Render(const engine::Transform&)
{

}

void GreenEnemyComponent::Move(engine::Direction direction)
{
	m_CurrentMoveCooldown = m_MoveCooldown;
	
	if (m_pCurrentNode.expired())
		return;

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<engine::Direction>(static_cast<size_t>(direction)));
	if (!temp.expired())
	{
		engine::DebugManager::GetInstance().print("Green enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
		m_pCurrentNode.lock()->Decrement();
	}
	else
		Die();
}

void GreenEnemyComponent::Die()
{
	m_pOwner.lock()->Destroy();
}