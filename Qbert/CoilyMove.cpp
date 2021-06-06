#include "MiniginPCH.h"
#include "CoilyMove.h"
#include "CoilyComponent.h"
#include "DebugManager.h"

CoilyMove::CoilyMove(const std::weak_ptr<CoilyComponent>& target, Direction direction)
	:BaseCommand()
	, m_pTarget{ target }
	, m_Direction{ direction }
{

}

void CoilyMove::Execute()
{
	engine::DebugManager::GetInstance().print("Move command was given for Coily with direction: " + std::to_string(static_cast<size_t>(m_Direction)), INPUT_DEBUG);
	if (!m_pTarget.expired())
		m_pTarget.lock()->Move(m_Direction);
}

void CoilyMove::ChangeTarget(const std::weak_ptr<CoilyComponent>& target)
{
	m_pTarget = target;
}