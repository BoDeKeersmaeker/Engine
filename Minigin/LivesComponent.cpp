#include "MiniginPCH.h"
#include "LivesComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

LivesComponent::LivesComponent(const std::shared_ptr<engine::GameObject>& owner, const std::shared_ptr<engine::Font>& font)
	:Component(owner)
{
	owner->AddComponent<engine::TextComponent>(std::make_shared<engine::TextComponent>(owner, "5 Lives left", font));
	m_pTextComponent = owner->GetComponent<engine::TextComponent>();
	m_pTextComponent.lock()->SetTextColor(engine::Color{ 255.f, 255.f, 50.f });
}

void LivesComponent::Update()
{
	
}

void LivesComponent::Render(const engine::Transform&)
{
	
}

void LivesComponent::SetLives(int lives) const
{
	if(lives > 0)
		m_pTextComponent.lock()->SetText(std::to_string(lives) + " Lives left");
	else
		m_pTextComponent.lock()->SetText("Game over.");
}
