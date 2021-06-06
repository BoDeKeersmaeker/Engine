#pragma once
#include "Component.h"
#include "Helpers.h"

namespace engine {
	class SubjectComponent;
	class RenderComponent; }
class PlayerComponent;
class GridNodeComponent;

class CoilyComponent final : public engine::Component
{
public:
	CoilyComponent(const std::shared_ptr<engine::GameObject>& owner, const std::pair<std::string, std::string>& texturePaths, bool IsAi = true, const std::weak_ptr<GridNodeComponent>& pStartNode = std::shared_ptr<GridNodeComponent>(nullptr), const std::weak_ptr<PlayerComponent>& pTarget = std::shared_ptr<PlayerComponent>(nullptr), float moveCooldown = 1.f);
	
	void Update() override;
	void Render(const engine::Transform& transform) override;

	std::weak_ptr<GridNodeComponent> GetCurrentNode() const;
	void Move(Direction direction);

private:
	Direction Chase() const;
	void Die() const;

	std::pair<std::string, std::string> m_TexturePaths{};
	std::weak_ptr<engine::RenderComponent>  m_pRenderComponent;
	std::weak_ptr<GridNodeComponent> m_pCurrentNode;
	std::weak_ptr<PlayerComponent> m_pTarget;
	std::weak_ptr<engine::SubjectComponent> m_pSubject;
	float m_CurrentMoveCooldown = 0;
	float m_MoveCooldown;
	bool m_Activated = false;
	bool m_IsAi;
};