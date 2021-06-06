#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include "Structs.h"

namespace engine {
	class SubjectComponent;
	class RenderComponent; }
class DiscComponent;

class GridNodeComponent : public engine::Component
{
public:
	GridNodeComponent(std::shared_ptr<engine::GameObject> owner, bool revertOverIncrement, const std::vector<std::string>& blockPaths);
	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

	std::weak_ptr<GridNodeComponent> GetConnection(engine::Direction connectionDirection);
	void SetConnection(engine::Direction connectionDirection, std::weak_ptr<GridNodeComponent> pThisNode, std::weak_ptr<GridNodeComponent> pOtherNode);
	void SetDiscs(std::pair<std::weak_ptr<DiscComponent>, std::weak_ptr<DiscComponent>> pDiscs);
	std::pair<std::weak_ptr<DiscComponent>, std::weak_ptr<DiscComponent>> GetDiscs() const;
	void Increment();
	void Decrement();
	bool IsCompleted() const;

private:
	std::vector<std::weak_ptr<GridNodeComponent>> m_pConnections;
	std::pair<std::weak_ptr<DiscComponent>, std::weak_ptr<DiscComponent>> m_pDiscs;
	std::vector<std::string> m_BlockPaths;
	std::weak_ptr<engine::SubjectComponent> m_pSubject;
	size_t m_CurrentBlockIndex = 0;
	std::weak_ptr<engine::RenderComponent>  m_pRenderComponent;
	bool m_RevertOverIncrement;
};