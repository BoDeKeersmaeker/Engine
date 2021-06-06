#include "MiniginPCH.h"
#include "CoilyComponent.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "RenderComponent.h"
#include "SubjectComponent.h"

CoilyComponent::CoilyComponent(std::shared_ptr<engine::GameObject> owner, const std::pair<std::string, std::string>& texturePaths, bool IsAi, std::weak_ptr<GridNodeComponent> pStartNode, std::weak_ptr<PlayerComponent> pTarget, float moveCooldown)
	:Component(owner)
	, m_TexturePaths{ texturePaths }
	, m_pCurrentNode{ pStartNode }
	, m_pTarget{ pTarget }
	, m_MoveCooldown{ moveCooldown }
	, m_IsAi{ IsAi }
{
	owner->AddComponent<engine::SubjectComponent>(std::make_shared<engine::SubjectComponent>(owner));
	m_pSubject = owner->GetComponent<engine::SubjectComponent>();
	
	owner->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(owner, m_TexturePaths.first, engine::Float2{ 0.f, -32.f }));
	m_pRenderComponent = owner->GetComponent<engine::RenderComponent>();
	m_pRenderComponent.lock()->SetTexture(m_TexturePaths.first);
}

void CoilyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= engine::EngineTime::GetInstance().GetElapsedSec();

	if(m_IsAi && m_Activated && m_CurrentMoveCooldown <= 0)
		Move(Chase());

	if(!m_Activated && m_CurrentMoveCooldown <= 0 )
		Move(static_cast<Direction>(rand() % 2 + 2));
}

void CoilyComponent::Render(const engine::Transform&)
{

}

std::weak_ptr<GridNodeComponent> CoilyComponent::GetCurrentNode() const
{
	return m_pCurrentNode;
}

Direction CoilyComponent::Chase()
{
	if (m_pTarget.expired())
		return Direction(0);
	
	auto ownerPos = m_pOwner.lock()->GetPosition();
	auto TargetPos = m_pTarget.lock()->GetCurrentNode().lock()->GetOwner().lock()->GetPosition();
	if(ownerPos.y < TargetPos.y)
	{
		if (ownerPos.x < TargetPos.x)
			return Direction::BOTTOMRIGHT;
		else
			return Direction::BOTTOMLEFT;
	}
	else
	{
		if (ownerPos.x < TargetPos.x)
			return Direction::TOPRIGHT;
		else
			return Direction::TOPLEFT;
	}
}

void CoilyComponent::Move(Direction direction)
{
	if (m_pTarget.expired())
	{
		m_pOwner.lock()->Destroy();
		return;
	}
	
	if(m_CurrentMoveCooldown > 0)
		return;

	auto tempTargetNode = m_pTarget.lock()->GetCurrentNode();
	if (m_IsAi && !tempTargetNode.expired() && m_pCurrentNode.lock() == tempTargetNode.lock() && m_pTarget.lock()->IsOnDisk())
		Die();

	m_CurrentMoveCooldown = m_MoveCooldown;

	if (m_pCurrentNode.expired())
	{
		m_pOwner.lock()->Destroy();
		return;
	}

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<Direction>(static_cast<size_t>(direction)));
	if (!temp.expired())
	{
		engine::DebugManager::GetInstance().print("Coily enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
	}
	else if(m_Activated && !m_IsAi)
		Die();
	else if(!m_Activated)
	{
		m_Activated = true;
		m_pRenderComponent.lock()->SetTexture(m_TexturePaths.second);
	}
}

void CoilyComponent::Die()
{
	engine::DebugManager::GetInstance().print("Coily was baited ", ENEMY_DEBUG);
	m_pSubject.lock()->Notify(engine::Event::ScoreChanged, 500);
	m_pOwner.lock()->Destroy();
}