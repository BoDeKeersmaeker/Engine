#include "MiniginPCH.h"
#include <iostream>
#include "ColorChange.h"
#include "../Qbert/PlayerComponent.h"
//#include "PlayerComponent.h"

engine::ColorChange::ColorChange(std::weak_ptr<PlayerComponent> target)
	:BaseCommand()
	, m_pTarget{ target }
{

}

void engine::ColorChange::Execute()
{
	if (!m_pTarget.expired())
	{
		std::cout << "Color of a block changed.\n";
		m_pTarget.lock()->ChangeScore(25);
	}
}