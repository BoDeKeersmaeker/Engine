#include "MiniginPCH.h"
#include <iostream>
#include "CatchSlick.h"
#include "../Qbert/PlayerComponent.h"
//#include "PlayerComponent.h"

engine::CatchSlick::CatchSlick(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{

}

void engine::CatchSlick::Execute()
{
	if (!m_pTarget.expired())
	{
		std::cout << "Slick caught.\n";
		m_pTarget.lock()->ChangeScore(500);
	}
}