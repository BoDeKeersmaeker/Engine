#include "MiniginPCH.h"
#include "Move.h"
#include "PlayerComponent.h"
#include "DebugManager.h"

Move::Move(std::weak_ptr<PlayerComponent> target, engine::Direction direction)
	:BaseCommand()
	, m_pTarget{ target }
	, m_Direction{ direction }
{

}

void Move::Execute()
{
	engine::DebugManager::GetInstance().print("Move command was given with direction: " + std::to_string(static_cast<size_t>(m_Direction)), INPUT_DEBUG);
	if (!m_pTarget.expired())
		m_pTarget.lock()->Move(m_Direction);
}