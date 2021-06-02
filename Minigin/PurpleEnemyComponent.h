#pragma once
#include "Component.h"
#include "Structs.h"

namespace engine
{
	class GridNodeComponent;

	class PurpleEnemyComponent : public Component
	{
	public:
		PurpleEnemyComponent(std::shared_ptr<GameObject> owner, bool moveLeft, std::weak_ptr<GridNodeComponent> pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), float moveCooldown = 1.f);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

	private:
		void Move(bool moveLeft);
		void Die();

		std::weak_ptr<GridNodeComponent> m_pCurrentNode;
		float m_MoveCooldown;
		float m_CurrentMoveCooldown;
	};
}