#include "MiniginPCH.h"
#include "Kill.h"
#include "PlayerComponent.h"

engine::Kill::Kill(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{
	
}

void engine::Kill::Execute()
{
	if(!m_pTarget.expired())
		m_pTarget.lock()->Die();
}
