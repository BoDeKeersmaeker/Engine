#pragma once
#include "Component.h"
#include "Structs.h"


class GridNodeComponent;

class PlayerComponent : public engine::Component
{
public:
	PlayerComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), float moveCooldown = 0.35f, int lives = 3);
	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

	void Move(engine::Direction direction);
	void Die();
	void SetSpawnNode(std::weak_ptr<GridNodeComponent> pNode);
	void SetCurrentNode(std::weak_ptr<GridNodeComponent> pNode);
	void SetIsOnDisk(bool isOnDisk);
	std::weak_ptr<GridNodeComponent> GetCurrentNode();
	bool GetIsOnDisk() const;
	void ChangeScore(int deltaScore);
	int GetLives() const;
	int GetScore() const;

private:
	void Respawn();
	
	std::weak_ptr<GridNodeComponent> m_pStartNode;
	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	float m_MoveCooldown;
	float m_CurrentMoveCooldown;
	int m_Lives;
	int m_Score = 0;
	bool m_IsOnDisk = false;
};