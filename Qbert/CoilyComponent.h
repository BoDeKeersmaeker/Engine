#pragma once
#include "Component.h"
#include "Structs.h"

namespace engine { class RenderComponent; }
class PlayerComponent;
class GridNodeComponent;

class CoilyComponent : public engine::Component
{
public:
	CoilyComponent(std::shared_ptr<engine::GameObject> owner, const std::pair<std::string, std::string>& texturePaths,std::weak_ptr<GridNodeComponent> pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), std::weak_ptr<PlayerComponent> pTarget = std::shared_ptr<PlayerComponent>(nullptr), float moveCooldown = 1.f);

	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

private:
	engine::Direction Chase();
	void Move(engine::Direction direction);
	void Die();

	std::pair<std::string, std::string> m_TexturePaths;
	std::weak_ptr<engine::RenderComponent>  m_pRenderComponent;
	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	std::weak_ptr<PlayerComponent> m_pTarget;
	float m_CurrentMoveCooldown = 0;
	float m_MoveCooldown;
	bool m_Activated = false;
};