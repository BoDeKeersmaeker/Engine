#pragma once
#include "Component.h"


namespace engine { class RenderComponent; }
class PlayerComponent;
class GridNodeComponent;

class CoilyComponents : public engine::Component
{
public:
	CoilyComponents(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<PlayerComponent> pTarget, float moveTime = 3.f);

	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

private:
	std::weak_ptr<engine::RenderComponent>  m_pRenderComponent;
	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	std::weak_ptr<PlayerComponent> m_pTarget;
	float m_CurrentMoveTime = 0;
	float m_MoveTime;
	bool m_IsActivated = false;
};