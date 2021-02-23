#pragma once
#include "Component.h"
namespace engine
{
	class Font;
	class Texture2D;
	class TextComponent;

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(const std::shared_ptr<Font>& font);

		virtual void Update() override;
		virtual void Render(const engine::Transform& transform) override;

	private:
		std::shared_ptr<TextComponent> m_pTextComponent;
		int m_FrameCount = 0;
		float m_ElapsedTime = 0.f;
	};
}

