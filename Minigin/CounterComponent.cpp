#include "MiniginPCH.h"
#include "CounterComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

engine::CounterComponent::CounterComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font, int defaultValue, std::string prefix, std::string sufix, std::string subZero)
	:Component(owner)
	, m_Prefix{ prefix }
	, m_Sufix{ sufix }
	, m_SubZero{ subZero }
{
	owner->AddComponent<TextComponent>(std::make_shared<TextComponent>(owner, prefix + std::to_string(defaultValue) + sufix, font));
	m_pTextComponent = owner->GetComponent<TextComponent>();
	m_pTextComponent.lock()->SetTextColor(Color{ 255.f, 255.f, 50.f });
}

void engine::CounterComponent::Update()
{

}

void engine::CounterComponent::Render(const engine::Transform&)
{

}

void engine::CounterComponent::SetCounter(int counter)
{
	if (m_SubZero != "" && counter <= 0)
		m_pTextComponent.lock()->SetText(m_SubZero);
	else
		m_pTextComponent.lock()->SetText(m_Prefix + std::to_string(counter) + m_Sufix);

}
