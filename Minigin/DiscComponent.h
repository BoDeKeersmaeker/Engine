#pragma once
#include "Component.h"


namespace engine { class RenderComponent; }
class PlayerComponent;
class GridNodeComponent;

class DiscComponent : public engine::Component
{
public:
	DiscComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<GridNodeComponent> pDestination, float moveTime = 3.f);

	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;
	
	void Activate(std::weak_ptr<PlayerComponent> pPlayer);

private:
	std::weak_ptr<engine::RenderComponent>  m_pRenderComponent;
	std::weak_ptr<GridNodeComponent> m_pDestination;
	std::weak_ptr<PlayerComponent> m_pPlayer;
	float m_CurrentMoveTime = 0;
	float m_MoveTime;
	bool m_IsActivated = false;
};