#pragma once
#include "Component.h"

namespace engine
{
	class Texture2D;
	class Transform;

	class RenderComponent final : public Component
	{
	public:
		RenderComponent(std::shared_ptr<GameObject> owner, std::string path);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

