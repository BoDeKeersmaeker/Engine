#pragma once
#include <string>
#include "Component.h"

namespace engine
{
	class Font;
	class TextComponent;

	class CounterComponent final : public Component
	{
	public:
		CounterComponent(std::shared_ptr<GameObject> owner, std::shared_ptr<Font> font, int defaultValue = 0, const std::string& prefix = "", const std::string& sufix = "", const std::string& subZero = "");

		void Update() override;
		void Render(const engine::Transform& transform) override;

		void SetCounter(int counter);

	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
		std::string m_Prefix;
		std::string m_Sufix;
		std::string m_SubZero;
	};
}
