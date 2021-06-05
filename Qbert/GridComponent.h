#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "Structs.h"
#include <regex>

namespace engine { class Scene; }

class GridNodeComponent;
class DiscComponent;

class GridComponent : public engine::Component
{
public:
	GridComponent(std::shared_ptr<engine::GameObject> owner, engine::Scene* scene, const std::string& filePath, size_t priority);
	
	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

	std::weak_ptr<GridNodeComponent> GetSoloStartNode() const { return m_pSoloStartNode; };
	const std::pair<std::weak_ptr<GridNodeComponent>, std::weak_ptr<GridNodeComponent>>& GetCoopStartNodes() const { return m_pCoopStartNodes; };
	bool IsCompleted() const;
	void Clear();

private:
	void ReadLevelFile(engine::Scene* scene, const std::string& filePath, size_t priority);
	size_t ReadSize_t(const std::string& input) const;
	float ReadFloat(const std::string& input) const;
	std::string ReadTexturePath(const std::string& input) const;
	bool ReadBool(const std::string& input) const;
	//int = layer, bool = isLeft
	std::pair<int, bool> ReadDisk(const std::string& input) const;
	static bool AreBlockTexturePathsValid(const std::vector<std::string>& blockTexturePaths);
	static bool AreDiskPositionsValid(const std::vector<std::pair<int, bool>>& diskPositions);

	void GenerateLevel(engine::Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, const std::vector<std::pair<int, bool>>& discPositions, size_t priority);
	void GenerateLayer(engine::Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, const std::vector<std::pair<int, bool>>& discPositions, const std::vector<std::weak_ptr<GridNodeComponent>>& pPreviousLayer, size_t priority);
	std::weak_ptr<GridNodeComponent> AddNode(engine::Scene* scene, engine::Float2 pos, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, size_t priority, std::weak_ptr<GridNodeComponent> m_pTopLeftConnection, std::weak_ptr<GridNodeComponent> m_pTopRightConnection);
	std::weak_ptr<DiscComponent> AddDisc(engine::Scene* scene, engine::Float2 pos, std::weak_ptr<GridNodeComponent> m_pTopNode, size_t priority);
	static bool IsDiscNeeded(const std::vector<std::pair<int, bool>>& diskPositions, size_t layer, bool needsLeft);

	std::vector<std::weak_ptr<GridNodeComponent>> m_pGrid;
	std::vector<std::weak_ptr<DiscComponent>> m_pDisks;
	std::regex m_GridRegex{ "(\\w+:)\\s(.+)" };
	std::regex m_Size_tRegex{ "(\\d+)$" };
	std::regex m_FloatRegex{ "(-?\\d+[\\.|\\,]?\\d?)" };
	std::regex m_TexturePathRegex{ "(\\w+.png)" };
	std::regex m_BoolRegex{ "(true|false)" };
	std::regex m_DiskRegex{ "(\\d+),\\s(\\w+)" };
	std::pair<std::weak_ptr<GridNodeComponent>, std::weak_ptr<GridNodeComponent>> m_pCoopStartNodes = { std::shared_ptr<GridNodeComponent>(nullptr), std::shared_ptr<GridNodeComponent>(nullptr) };
	std::weak_ptr<GridNodeComponent> m_pSoloStartNode = std::shared_ptr<GridNodeComponent>(nullptr);
	size_t m_MaxLayers = 0;
};