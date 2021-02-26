#pragma once
#include "Component.h"

namespace engine
{
	class Font;
	class TextComponent;
	
	class ScoreComponent : public Component
	{
	public:
		ScoreComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

		void SetScore(int deltaScore);
	
	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
	};
}

