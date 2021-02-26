#include "MiniginPCH.h"
#include "RemainingDisc.h"
#include "PlayerComponent.h"

engine::RemainingDisc::RemainingDisc(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{

}

void engine::RemainingDisc::Execute()
{
	if (!m_pTarget.expired())
	{
		std::cout << "Disc remaining\n";
		m_pTarget.lock()->ChangeScore(50);
	}
}
