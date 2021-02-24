#include "MiniginPCH.h"
#include "Reaper.h"

engine::Reaper::Reaper()
	:Observer()
{
	
}

void engine::Reaper::OnNotify(const GameObject& gameObject, Event event)
{
	switch (event)
	{
	case Event::GameObjectDestroyed:
		gameObject;
		std::cout << "Player dead notification.\n";
		break;
	}
}
