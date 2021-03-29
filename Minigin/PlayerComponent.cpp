#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "GameObject.h"
#include "Observer.h"
#include "SubjectComponent.h"
#include "AudioLocator.h"
#include "Audio.h"

engine::PlayerComponent::PlayerComponent(std::shared_ptr<GameObject> owner, int lives)
	:Component(owner)
	,m_Lives{ lives }
{
	owner->AddComponent<SubjectComponent>(std::make_shared<SubjectComponent>(owner));
	Respawn();
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

	AudioLocator::getAudioSystem()->play(0);
	
	Respawn();
	
	if (m_Lives  <= 0)
		m_pOwner.lock()->Destroy();
}

void engine::PlayerComponent::ChangeScore(int deltaScore)
{
	m_Score += deltaScore;
	m_pOwner.lock()->GetComponent<SubjectComponent>().lock()->Notify(m_pOwner, Event::ScoreChanged);
}

int engine::PlayerComponent::GetLives() const
{
	return m_Lives;
}

int engine::PlayerComponent::GetScore() const
{
	return m_Score;
}

void engine::PlayerComponent::Respawn() const
{
	//temp code for fun
	const auto x = static_cast<float>(rand() % 641);
	const auto y = static_cast<float>(rand() % 480);
	m_pOwner.lock()->SetPosition(x, y);
}
