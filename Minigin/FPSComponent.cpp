#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "Time.h"

engine::FPSComponent::FPSComponent(const std::shared_ptr<Font>& font)
	: Component(ComponentType::FPS)
	,m_pTextComponent(std::make_shared<TextComponent>("60 FPS", font))
{
	m_pTextComponent->SetTextColor(Color{ 255.f, 255.f, 50.f });
}

void engine::FPSComponent::Update()
{
	m_FrameCount++;
	m_ElapsedTime += Time::GetInstance().GetElapsedSec();

	if (m_ElapsedTime >= 1.f)
	{
		m_pTextComponent->SetText(std::to_string(m_FrameCount) + " FPS");
		m_ElapsedTime -= 1.f;
		m_FrameCount = 0;
	}

	m_pTextComponent->Update();
}

void engine::FPSComponent::Render(const engine::Transform& transform)
{
	m_pTextComponent->Render(transform);
}
