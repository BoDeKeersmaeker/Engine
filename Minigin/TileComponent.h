#pragma once
#include <string>
#include <vector>
#include "Component.h"

namespace engine
{
	class Font;
	class TextComponent;

	class TileComponent : public Component
	{
	public:
		TileComponent(std::shared_ptr<GameObject> owner, std::vector<std::string> textures);

		virtual void Update() override;
		virtual void Render(const engine::Transform& transform) override;
	
	private:
		std::vector<std::weak_ptr<TextComponent>> m_pTextComponent;
	};
}