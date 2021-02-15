#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "Time.h"

dae::FPSComponent::FPSComponent(const std::shared_ptr<Font>& font)
	: Component(ComponentType::FPS)
	,m_pTextComponent(std::make_shared<TextComponent>("60 FPS", font))
{
	m_pTextComponent->SetTextColor(Color{ 255.f, 255.f, 50.f });
}

void dae::FPSComponent::Update()
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

void dae::FPSComponent::Render(const dae::Transform& transform)
{
	m_pTextComponent->Render(transform);
}
