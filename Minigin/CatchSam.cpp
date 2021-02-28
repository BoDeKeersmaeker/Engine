#include "MiniginPCH.h"
#include "CatchSam.h"
#include "PlayerComponent.h"

engine::CatchSam::CatchSam(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{

}

void engine::CatchSam::Execute()
{
	if (!m_pTarget.expired())
	{
		std::cout << "Sam caught.\n";
		m_pTarget.lock()->ChangeScore(500);
	}
}