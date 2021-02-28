#pragma once
#include "Component.h"

namespace engine
{
	class Font;
	class TextComponent;

	class CounterComponent : public Component
	{
	public:
		CounterComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font, int defaultValue = 0, std::string prefix = "", std::string sufix = "", std::string subZero = "");

		virtual void Update() override;
		virtual void Render(const engine::Transform& transform) override;

		void SetCounter(int counter);

	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
		std::string m_Prefix;
		std::string m_Sufix;
		std::string m_SubZero;
	};
}
