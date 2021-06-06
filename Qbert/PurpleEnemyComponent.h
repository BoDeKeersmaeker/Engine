#pragma once
#include "Component.h"
#include "Helpers.h"

namespace engine {
	class SubjectComponent;
}

class GridNodeComponent;

class PurpleEnemyComponent : public engine::Component
{
public:
	PurpleEnemyComponent(std::shared_ptr<engine::GameObject> owner, bool moveLeft, std::weak_ptr<GridNodeComponent> pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), float moveCooldown = 1.f);
	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

	std::weak_ptr<GridNodeComponent> GetCurrentNode() const;
	bool CheckOverlap(std::weak_ptr<GridNodeComponent> node) const;

private:
	void Move(Direction direction);
	void Die() const;

	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	std::weak_ptr<engine::SubjectComponent> m_pSubject;
	float m_MoveCooldown;
	float m_CurrentMoveCooldown;
	bool m_MoveLeft;
};