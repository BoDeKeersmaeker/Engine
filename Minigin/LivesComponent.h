#pragma once
#include "Component.h"

namespace engine
{
	class Font;
	class TextComponent;
	
	class LivesComponent : public Component
	{
	public:
		LivesComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font>& font);

		virtual void Update() override;
		virtual void Render(const engine::Transform& transform) override;

		void SetLives(int lives);

	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
	};
}