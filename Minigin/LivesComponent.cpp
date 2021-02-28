#include "MiniginPCH.h"
#include "LivesComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

engine::LivesComponent::LivesComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font)
	:Component(owner)
{
	owner->AddComponent<TextComponent>(std::make_shared<TextComponent>(owner, "5 Lives left", font));
	m_pTextComponent = owner->GetComponent<TextComponent>();
	m_pTextComponent.lock()->SetTextColor(Color{ 255.f, 255.f, 50.f });
}

void engine::LivesComponent::Update()
{
	
}

void engine::LivesComponent::Render(const engine::Transform&)
{
	
}

void engine::LivesComponent::SetLives(int lives)
{
	if(lives > 0)
		m_pTextComponent.lock()->SetText(std::to_string(lives) + " Lives left");
	else
		m_pTextComponent.lock()->SetText("Game over.");
}