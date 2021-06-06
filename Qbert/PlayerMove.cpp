#include "MiniginPCH.h"
#include "PlayerMove.h"
#include "PlayerComponent.h"
#include "DebugManager.h"

PlayerMove::PlayerMove(std::weak_ptr<PlayerComponent> target, Direction direction)
	:BaseCommand()
	, m_pTarget{ target }
	, m_Direction{ direction }
{

}

void PlayerMove::Execute()
{
	engine::DebugManager::GetInstance().print("Move command was given for Qbert with direction: " + std::to_string(static_cast<size_t>(m_Direction)), INPUT_DEBUG);
	if (!m_pTarget.expired())
		m_pTarget.lock()->Move(m_Direction);
}