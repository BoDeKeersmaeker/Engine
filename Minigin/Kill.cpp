#include "MiniginPCH.h"
#include "Kill.h"

#include <mutex>

#include "GameObject.h"
#include "PlayerComponent.h"

engine::Kill::Kill(std::weak_ptr<GameObject> target)
	:BaseCommand()
	, m_pTarget{ target }
{
	
}

void engine::Kill::Execute()
{
	if (m_pTarget.expired())
		return;
	
	auto temp = m_pTarget.lock()->GetComponent<PlayerComponent>();
	if(!temp.expired())
		temp.lock()->Die();
}
