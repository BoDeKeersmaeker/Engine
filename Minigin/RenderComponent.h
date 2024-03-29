#pragma once
#include <string>
#include "Component.h"
#include "Structs.h"

namespace engine
{
	class Texture2D;
	class Transform;

	class RenderComponent final : public Component
	{
	public:
		RenderComponent(const std::shared_ptr<GameObject>& owner, const std::string& path, Float2 offset = {0.f, 0.f});
		void Update() override;
		void Render(const Transform& transform) override;
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
		Float2 m_Offset;
	};
}

