#pragma once
#include <string>

#include "Component.h"
#include "Structs.h"

namespace engine
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		explicit TextComponent(const std::shared_ptr<GameObject>& owner, const std::string& text, const std::shared_ptr<Font>& font);
		void SetText(const std::string& text);
		void SetTextColor(Color color) { m_TextColor = color; };

		void Update() override;
		void Render(const engine::Transform& transform) override;

	private:
		bool m_NeedsUpdate;
		std::shared_ptr<Texture2D> m_Texture;
		std::shared_ptr<Font> m_Font;
		std::string m_Text;
		Color m_TextColor;
	};
}