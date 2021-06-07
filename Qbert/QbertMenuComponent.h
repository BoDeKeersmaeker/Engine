#pragma once
#include "Component.h"
#include "Helpers.h"

namespace engine {
	class Scene;
}

class QbertMenuComponent final : public engine::Component
{
public:
	QbertMenuComponent(const std::shared_ptr<engine::GameObject>& owner, const std::weak_ptr<engine::Scene>& scene);

	void Update() override;
	void Render(const engine::Transform& transform) override;

private:
	bool m_ShowHowToPlay = true;
	bool m_ShowStartMenu = true;
	std::weak_ptr<engine::Scene> m_pScene;
};
