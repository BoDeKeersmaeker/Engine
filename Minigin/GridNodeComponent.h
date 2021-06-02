#pragma once
#include "Component.h"
#include <string>
#include <vector>

#include "Structs.h"

namespace engine
{
	class RenderComponent;
	
	class GridNodeComponent : public Component
	{
	public:
		GridNodeComponent(std::shared_ptr<GameObject> owner, bool revertOverIncrement, const std::vector<std::string>& blockPaths);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

		std::weak_ptr<GridNodeComponent> GetConnection(Direction connectionDirection);
		void SetConnection(Direction connectionDirection, std::weak_ptr<GridNodeComponent> pThisNode, std::weak_ptr<GridNodeComponent> pOtherNode);
		void Increment();
		void Decrement();
		bool IsCompleted() const;
	
	private:
		std::vector<std::weak_ptr<GridNodeComponent>> m_pConnections;
		std::vector<std::string> m_BlockPaths;
		size_t m_CurrentBlockIndex = 0;
		std::weak_ptr<RenderComponent>  m_pRenderComponent;
		bool m_RevertOverIncrement;
	};
}
