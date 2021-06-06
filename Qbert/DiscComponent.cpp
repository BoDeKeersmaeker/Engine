#include "MiniginPCH.h"
#include "DiscComponent.h"

#include "EngineTime.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "PlayerComponent.h"
#include "RenderComponent.h"

DiscComponent::DiscComponent(const std::shared_ptr<engine::GameObject>& owner, const std::weak_ptr<GridNodeComponent>& pDestination, float moveTime)
	:Component(owner)
	, m_pDestination{ pDestination }
	, m_MoveTime{ moveTime }
{
	owner->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(owner, "Disc.png"));
	m_pRenderComponent = owner->GetComponent<engine::RenderComponent>();
	m_pRenderComponent.lock()->SetTexture("Disc.png");
}

void DiscComponent::Update() 
{
	if(m_IsActivated && m_CurrentMoveTime >= 0)
	{
		m_CurrentMoveTime -= engine::EngineTime::GetInstance().GetElapsedSec();
		if(m_CurrentMoveTime <= 0)
		{
			engine::DebugManager::GetInstance().print("Disc completed.", DISC_DEBUG);
			auto tempPlayer = m_pPlayer.lock();
			tempPlayer->GetOwner().lock()->SetPosition(m_pDestination.lock()->GetOwner().lock()->GetPosition());
			tempPlayer->SetCurrentNode(m_pDestination);
			tempPlayer->SetIsOnDisk(false);
			m_pDestination.lock()->Increment();
			m_pOwner.lock()->Destroy();
		}
	}
}

void DiscComponent::Render(const engine::Transform&)
{

}

void DiscComponent::Activate(const std::weak_ptr<PlayerComponent>& pPlayer)
{
	if (!m_IsActivated && !pPlayer.expired())
	{
		engine::DebugManager::GetInstance().print("Disc activated.", DISC_DEBUG);
		m_pPlayer = pPlayer;
		m_pPlayer.lock()->GetOwner().lock()->SetPosition(m_pOwner.lock()->GetPosition());
		m_IsActivated = true;
		m_pPlayer.lock()->SetIsOnDisk(true);
		m_CurrentMoveTime = m_MoveTime;
	}
}