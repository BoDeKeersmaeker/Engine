#pragma once
#include "Component.h"

namespace engine {
	class Font;
	class TextComponent;
}
	
class ScoreComponent final : public engine::Component
{
public:
	ScoreComponent(const std::shared_ptr<engine::GameObject>& owner, const std::shared_ptr<engine::Font>& font);
	void Update() override;
	void Render(const engine::Transform& transform) override;

	void SetScore(int deltaScore) const;
	
private:
	std::weak_ptr<engine::TextComponent> m_pTextComponent;
};