#include "MiniginPCH.h"
#include "Component.h"

engine::Component::Component(std::shared_ptr<GameObject> owner)
	:m_pOwner(owner)
{

}