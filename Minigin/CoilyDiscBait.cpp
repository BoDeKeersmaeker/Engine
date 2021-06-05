#include "MiniginPCH.h"
#include <iostream>
#include "CoilyDiscBait.h"
#include "../Qbert/PlayerComponent.h"
//#include "PlayerComponent.h"

engine::CoilyDiscBait::CoilyDiscBait(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{

}

void engine::CoilyDiscBait::Execute()
{
	if (!m_pTarget.expired())
	{
		std::cout << "Coily fell of the map.\n";
		m_pTarget.lock()->ChangeScore(500);
	}
}