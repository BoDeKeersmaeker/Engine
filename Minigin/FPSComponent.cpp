#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "EngineTime.h"

engine::FPSComponent::FPSComponent(const std::shared_ptr<GameObject>& owner, std::shared_ptr<Font> font)
	: Component(owner)
{
	owner->AddComponent<TextComponent>(std::make_shared<TextComponent>(owner, "60 FPS", font));
	m_pTextComponent = owner->GetComponent<TextComponent>();
	
	m_pTextComponent.lock()->SetTextColor(Color{ 255.f, 255.f, 50.f });
}

void engine::FPSComponent::Update()
{
	m_FrameCount++;
	m_ElapsedTime += EngineTime::GetInstance().GetElapsedSec();

	if (m_ElapsedTime >= 1.f)
	{
		m_pTextComponent.lock()->SetText(std::to_string(m_FrameCount) + " FPS");
		m_ElapsedTime -= 1.f;
		m_FrameCount = 0;
	}
}

void engine::FPSComponent::Render(const engine::Transform&)
{
	
}
