#pragma once
#include "Component.h"
#include "Structs.h"

namespace engine
{
	class GridNodeComponent;

	class PlayerComponent : public Component
	{
	public:
		PlayerComponent(std::shared_ptr<GameObject> owner, std::weak_ptr<GridNodeComponent> pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), float moveCooldown = 0.35f, int lives = 3);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

		void Move(MoveDirection direction);
		void Die();
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
	};
}