#pragma once
#include "Component.h"
namespace engine
{
	class Font;
	class TextComponent;

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(const std::shared_ptr<GameObject>& owner, std::shared_ptr<Font> font);

		void Update() override;
		void Render(const engine::Transform& transform) override;

	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
		int m_FrameCount = 0;
		float m_ElapsedTime = 0.f;
	};
}

