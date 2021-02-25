#pragma once
#include "Component.h"

namespace engine
{
	class PlayerComponent : public Component
	{
	public:
		PlayerComponent(std::shared_ptr<GameObject> owner, int playerId,int lives);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;
		
		void Die();
		int GetLives();
	
	private:
		int m_PlayerId;
		int m_Lives;
	};
}

