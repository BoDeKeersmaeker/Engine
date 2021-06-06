#include "MiniginPCH.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

ScoreComponent::ScoreComponent(const std::shared_ptr<engine::GameObject>& owner, const std::shared_ptr<engine::Font>& font)
	:Component(owner)
{
	owner->AddComponent<engine::TextComponent>(std::make_shared<engine::TextComponent>(owner, "Score: 0", font));
	m_pTextComponent = owner->GetComponent<engine::TextComponent>();
	m_pTextComponent.lock()->SetTextColor(engine::Color{ 255.f, 255.f, 50.f });
}

void ScoreComponent::Update()
{
	
}

void ScoreComponent::Render(const engine::Transform&)
{
	
}

void ScoreComponent::SetScore(int Score) const
{
	m_pTextComponent.lock()->SetText("Score: " + std::to_string(Score));
}
