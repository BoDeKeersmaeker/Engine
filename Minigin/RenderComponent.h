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
		RenderComponent(std::shared_ptr<GameObject> owner, std::string path, Float2 offset = {0.f, 0.f});
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
		Float2 m_Offset;
	};
}

