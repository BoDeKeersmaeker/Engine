#pragma once
#include "Component.h"
#include "Helpers.h"

namespace engine {
	class SubjectComponent;
}

class GridNodeComponent;

class PlayerComponent final : public engine::Component
{
public:
	PlayerComponent(const std::shared_ptr<engine::GameObject>& owner, const std::weak_ptr<GridNodeComponent>& pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), float moveCooldown = 0.35f, int lives = 3);
	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

	void Move(Direction direction);
	void Die();
	void SetCurrentNode(const std::weak_ptr<GridNodeComponent>& pNode);
	void SetIsOnDisk(bool isOnDisk);
	std::weak_ptr<GridNodeComponent> GetCurrentNode() const;
	bool IsOnDisk() const;
	int GetLives() const;
	void Reset(const std::weak_ptr<GridNodeComponent>& newStartNode = std::shared_ptr<GridNodeComponent>(nullptr));

private:
	void Respawn();

	std::weak_ptr<engine::SubjectComponent> m_pSubject;
	std::weak_ptr<GridNodeComponent> m_pStartNode;
	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	float m_MoveCooldown;
	float m_CurrentMoveCooldown;
	int m_Lives;
	bool m_IsOnDisk = false;
};