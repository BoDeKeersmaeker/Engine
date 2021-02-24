#pragma once
#include "Component.h"

namespace engine
{
	class RenderComponent;

	class TextureComponent final : public Component
	{
	public:
		TextureComponent(std::shared_ptr<GameObject> owner);
		void SetTexture(const std::string& filename);

		virtual void Update() override;
		virtual	void Render(const Transform& transform) override;

	private:
		std::shared_ptr<RenderComponent> m_RenderComp;
	};
}
