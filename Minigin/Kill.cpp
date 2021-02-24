#include "MiniginPCH.h"
#include "Kill.h"

engine::Kill::Kill(std::shared_ptr<GameObject> target)
	:BaseCommand()
	, m_pTarget{ target }
{
	
}

void engine::Kill::Execute()
{
	std::cout << "Kill called.\n";
	
}
