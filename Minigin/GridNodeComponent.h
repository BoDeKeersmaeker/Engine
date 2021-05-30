#pragma once
#include "Component.h"

namespace engine
{
	class RenderComponent;

	enum class ConnectionDirection
	{
		TOPLEFT = 0,
		TOPRIGHT = 1,
		BOTTOMRIGHT = 2,
		BOTTOMLEFT= 3,
	};
	
	class GridNodeComponent : public Component
	{
	public:
		GridNodeComponent(std::shared_ptr<GameObject> owner, const std::vector<std::string>& blockPaths);
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

		const std::weak_ptr<GridNodeComponent>& GetConnection(ConnectionDirection connectionDirection);
		void SetConnection(ConnectionDirection connectionDirection, std::weak_ptr<GridNodeComponent> pOtherNode);

	private:
		std::vector<std::weak_ptr<GridNodeComponent>> m_pConnections;
		std::vector<std::string> m_BlockPaths;
		std::weak_ptr<RenderComponent>  m_pRenderComponent;
	};
}