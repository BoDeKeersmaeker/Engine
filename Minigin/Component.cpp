#include "MiniginPCH.h"
#include "Component.h"

engine::Component::Component(const std::shared_ptr<GameObject>& owner)
	:m_pOwner(owner)
{

}