#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "Structs.h"

namespace engine
{
	class GridNodeComponent;
	class Scene;

	class GridComponent : public Component
	{
	public:
		GridComponent(std::shared_ptr<GameObject> owner, const std::vector<std::string>& blockPaths);
		
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

		void GenerateGrid(Scene* scene, size_t amountOfLayers, float width, float height);
		
	private:
		void GenerateLayer(Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::weak_ptr<GridNodeComponent>>& pPreviousLayer);
		std::weak_ptr<GridNodeComponent> AddNode(Scene* scene, Float2 pos, std::weak_ptr<GridNodeComponent> m_pTopLeftConnection, std::weak_ptr<GridNodeComponent> m_pTopRightConnection);
		
		std::vector<std::weak_ptr<GridNodeComponent>> m_pGrid;
		std::vector<std::string> m_BlockPaths;
	};
}
