#include "MiniginPCH.h"
#include "ColorChange.h"
#include "PlayerComponent.h"

engine::ColorChange::ColorChange(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{

}

void engine::ColorChange::Execute()
{
	if (!m_pTarget.expired())
		m_pTarget.lock()->ChangeScore(25);
}
