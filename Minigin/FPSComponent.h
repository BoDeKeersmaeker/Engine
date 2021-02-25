#pragma once
#include "Component.h"
namespace engine
{
	class Font;
	class TextComponent;

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font);

		virtual void Update() override;
		virtual void Render(const engine::Transform& transform) override;

	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
		int m_FrameCount = 0;
		float m_ElapsedTime = 0.f;
	};
}

