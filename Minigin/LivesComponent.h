#pragma once
#include "Component.h"

namespace engine {
	class Font;
	class TextComponent;
}

class LivesComponent final : public engine::Component
{
public:
	LivesComponent(const std::shared_ptr<engine::GameObject>& owner, const std::shared_ptr<engine::Font>& font);

	void Update() override;
	void Render(const engine::Transform& transform) override;

	void SetLives(int lives) const;
private:
	std::weak_ptr<engine::TextComponent> m_pTextComponent;
};
