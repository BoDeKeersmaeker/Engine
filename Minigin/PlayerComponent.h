#pragma once
#include "Component.h"

namespace engine
{
	class PlayerComponent : public Component
	{
	public:
		PlayerComponent(std::shared_ptr<GameObject> owner,int lives = 5);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;
		
		void Die();
		void ChangeScore(int deltaScore);
		int GetLives() const;
		int GetScore() const;
	
	private:
		void Respawn() const;
		
		int m_Lives;
		int m_Score = 0;
	};
}

