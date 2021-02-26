#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

engine::ScoreComponent::ScoreComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font)
	:Component(owner)
{
	owner->AddComponent<TextComponent>(std::make_shared<TextComponent>(owner, "Score: 0", font));
	m_pTextComponent = owner->GetComponent<TextComponent>();
	m_pTextComponent.lock()->SetTextColor(Color{ 255.f, 255.f, 50.f });
}

void engine::ScoreComponent::Update()
{
	
}

void engine::ScoreComponent::Render(const Transform&)
{
	
}

void engine::ScoreComponent::SetScore(int Score)
{
	m_pTextComponent.lock()->SetText("Score: " + std::to_string(Score));
}
