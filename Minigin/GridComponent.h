#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "Structs.h"
#include <regex>

namespace engine
{
	class GridNodeComponent;
	class Scene;

	class GridComponent : public Component
	{
	public:
		GridComponent(std::shared_ptr<GameObject> owner, Scene* scene, const std::string& filePath);
		
		virtual void Update() override;
		virtual void Render(const Transform& transform) override;

		std::weak_ptr<GridNodeComponent> GetSoloStartNode() const { return m_pSoloStartNode; };
		const std::pair<std::weak_ptr<GridNodeComponent>, std::weak_ptr<GridNodeComponent>>& GetCoopStartNodes() const { return m_pCoopStartNodes; };
	
	private:
		void ReadLevelFile(Scene* scene, const std::string& filePath);
		bool IsBlockTexturePathsValid(const std::vector<std::string>& blockTexturePaths);
		void GenerateLevel(Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement);
		void GenerateLayer(Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, const std::vector<std::weak_ptr<GridNodeComponent>>& pPreviousLayer);
		std::weak_ptr<GridNodeComponent> AddNode(Scene* scene, Float2 pos, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, std::weak_ptr<GridNodeComponent> m_pTopLeftConnection, std::weak_ptr<GridNodeComponent> m_pTopRightConnection);

		size_t ReadSize_t(const std::string& input) const;
		float ReadFloat(const std::string& input) const;
		std::string ReadTexturePath(const std::string& input) const;
		bool ReadBool(const std::string& input) const;
	
		std::vector<std::weak_ptr<GridNodeComponent>> m_pGrid;
		std::regex m_GridRegex{ "(\\w+:)\\s(.+)" };
		std::regex m_Size_tRegex{ "(\\d+)$" };
		std::regex m_FloatRegex{ "(-?\\d+[\\.|\\,]?\\d?)" };
		std::regex m_TexturePathRegex{ "(\\w+.png)" };
		std::regex m_BoolRegex{ "(true|false)" };
		std::weak_ptr<GridNodeComponent> m_pSoloStartNode = std::shared_ptr<GridNodeComponent>(nullptr);
		std::pair<std::weak_ptr<GridNodeComponent>, std::weak_ptr<GridNodeComponent>> m_pCoopStartNodes = { std::shared_ptr<GridNodeComponent>(nullptr), std::shared_ptr<GridNodeComponent>(nullptr) };
	};
}