#include "MiniginPCH.h"
#include "CoilyComponent.h"
#include "GridNodeComponent.h"
#include "DebugManager.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "RenderComponent.h"

CoilyComponent::CoilyComponent(std::shared_ptr<engine::GameObject> owner, const std::pair<std::string, std::string>& texturePaths, std::weak_ptr<GridNodeComponent> pStartNode, std::weak_ptr<PlayerComponent> pTarget, float moveCooldown)
	:Component(owner)
	, m_TexturePaths{ texturePaths }
	, m_pCurrentNode{ pStartNode }
	, m_pTarget{ pTarget }
	, m_MoveCooldown{ moveCooldown }
{
	owner->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(owner, m_TexturePaths.first, engine::Float2{ 0.f, -32.f }));
	m_pRenderComponent = owner->GetComponent<engine::RenderComponent>();
	m_pRenderComponent.lock()->SetTexture(m_TexturePaths.first);
}

void CoilyComponent::Update()
{
	if (m_CurrentMoveCooldown >= 0.f)
		m_CurrentMoveCooldown -= engine::EngineTime::GetInstance().GetElapsedSec();

	if(m_Activated && m_CurrentMoveCooldown <= 0)
		Move(Chase());
	else if (m_CurrentMoveCooldown <= 0)
		Move(static_cast<engine::Direction>(rand() % 2 + 2));
}

void CoilyComponent::Render(const engine::Transform&)
{

}

engine::Direction CoilyComponent::Chase()
{
	if (m_pTarget.expired())
		return engine::Direction(0);
	
	auto ownerPos = m_pOwner.lock()->GetPosition();
	auto TargetPos = m_pTarget.lock()->GetCurrentNode().lock()->GetOwner().lock()->GetPosition();
	if(ownerPos.y < TargetPos.y)
	{
		if (ownerPos.x < TargetPos.x)
			return engine::Direction::BOTTOMRIGHT;
		else
			return engine::Direction::BOTTOMLEFT;
	}
	else
	{
		if (ownerPos.x < TargetPos.x)
			return engine::Direction::TOPRIGHT;
		else
			return engine::Direction::TOPLEFT;
	}
}

void CoilyComponent::Move(engine::Direction direction)
{
	if(m_pTarget.expired())
		return;
	
	auto tempTargetNode = m_pTarget.lock()->GetCurrentNode();
	if (!tempTargetNode.expired() && m_pCurrentNode.lock() == tempTargetNode.lock() && m_pTarget.lock()->GetIsOnDisk())
		Die();
	
	m_CurrentMoveCooldown = m_MoveCooldown;

	if (m_pCurrentNode.expired())
		return;

	auto temp = m_pCurrentNode.lock()->GetConnection(static_cast<engine::Direction>(static_cast<size_t>(direction)));
	if (!temp.expired())
	{
		engine::DebugManager::GetInstance().print("Coily enemy moved: " + std::to_string(static_cast<size_t>(direction)), ENEMY_DEBUG);
		m_pOwner.lock()->SetPosition(temp.lock()->GetOwner().lock()->GetPosition());
		m_pCurrentNode = temp;
	}
	else
	{
		m_Activated = true;
		m_pRenderComponent.lock()->SetTexture(m_TexturePaths.second);
	}
	
}

void CoilyComponent::Die()
{
	m_pOwner.lock()->Destroy();
}