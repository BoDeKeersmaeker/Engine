#include "MiniginPCH.h"
#include "PlayerComponent.h"

#include "GameObject.h"
#include "Observer.h"
#include "SubjectComponent.h"

engine::PlayerComponent::PlayerComponent(std::shared_ptr<GameObject> owner, int playerId, int lives)
	:Component(owner)
	,m_PlayerId{ playerId }
	,m_Lives{ lives }
{
	owner->AddComponent<SubjectComponent>(std::make_shared<SubjectComponent>(owner));
}

void engine::PlayerComponent::Update()
{
	
}

void engine::PlayerComponent::Render(const Transform&)
{
	
}

void engine::PlayerComponent::Die()
{
	m_Lives--;
	m_pOwner.lock()->GetComponent<SubjectComponent>().lock()->Notify(m_pOwner, Event::PlayerDied);
	
	if (m_Lives  <= 0)
		m_pOwner.lock()->Destroy();
}

int engine::PlayerComponent::GetLives()
{
	return m_Lives;
}
