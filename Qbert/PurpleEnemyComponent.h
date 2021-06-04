#pragma once
#include "Component.h"
#include "Structs.h"

class GridNodeComponent;

class PurpleEnemyComponent : public engine::Component
{
public:
	PurpleEnemyComponent(std::shared_ptr<engine::GameObject> owner, bool moveLeft, std::weak_ptr<GridNodeComponent> pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), float moveCooldown = 1.f);
	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

private:
	void Move(engine::Direction direction);
	void Die() const;

	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	float m_MoveCooldown;
	float m_CurrentMoveCooldown;
	bool m_MoveLeft;
};