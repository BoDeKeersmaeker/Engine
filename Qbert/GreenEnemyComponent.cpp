#include "MiniginPCH.h"
#include "GreenEnemyComponent.h"

#include "Audio.h"
#include "AudioLocator.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"
#include "Observer.h"
#include "SubjectComponent.h"

GreenEnemyComponent::GreenEnemyComponent(const std::shared_ptr<engine::GameObject>& owner, const std::weak_ptr<GridNodeComponent>& pStartNode, float moveCooldown)
	:Component(owner)
	, m_pCurrentNode{ pStartNode }
	, m_MoveCooldown{ moveCooldown }
	, m_CurrentMoveCooldown{ moveCooldown }
{
	owner->AddComponent<engine::SubjectComponent>(std::make_shared<engine::SubjectComponent>(owner));
	m_pSubject = owner->GetComponent<engine::SubjectComponent>();
	
	if (!m_pCurrentNode.expired())
		m_pOwner.lock()->SetPosition(m_pCurrentNode.lock()->GetOwner().lock()->GetPosition());
}

void GreenEnemyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= engine::EngineTime::GetInstance().GetElapsedSec();

	if(m_CurrentMoveCooldown <= 0)
		Move(static_cast<Direction>(rand() % 2 + 2));
}

void GreenEnemyComponent::Render(const engine::Transform&)
{

}

std::weak_ptr<GridNodeComponent> GreenEnemyComponent::GetCurrentNode() const
{
	return m_pCurrentNode;
}

bool GreenEnemyComponent::CheckOverlap(std::weak_ptr<GridNodeComponent> node) const
{
	if(m_pCurrentNode.lock() == node.lock())
	{
		m_pSubject.lock()->Notify(engine::Event::ScoreChanged, 300);
		return true;
	}
	return false;
}

void GreenEnemyComponent::Move(Direction direction)
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
		engine::DebugManager::GetInstance().print("Green enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
		m_pCurrentNode.lock()->Decrement();
	}
	else
		Die();
}

void GreenEnemyComponent::Die() const
{
	m_pOwner.lock()->Destroy();
}