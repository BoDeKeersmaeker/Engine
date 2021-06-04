#include "MiniginPCH.h"
#include "GridComponent.h"
#include <fstream>
#include "DebugManager.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "DiscComponent.h"
#include "Scene.h"

GridComponent::GridComponent(std::shared_ptr<engine::GameObject> owner, engine::Scene* scene, const std::string& filePath)
	:Component(owner)
	, m_pGrid{}
{
	ReadLevelFile(scene, filePath);
}

void GridComponent::Update()
{

}

void GridComponent::Render(const engine::Transform&)
{

}

void GridComponent::ReadLevelFile(engine::Scene* scene, const std::string& filePath)
{
	std::ifstream in{ filePath };
	if (!in)
	{
		engine::DebugManager::GetInstance().print("Level file not opened for reading.", GRID_DEBUG);
		return;
	}

	size_t amountOfLayers = 0;
	float width = 0;
	float height = 0;
	std::vector<std::string> blockTexturePaths;
	bool revertOverIncrement = false;
	std::vector<std::pair<int, bool>> diskPositions;


	engine::DebugManager::GetInstance().print("reading from Level file: ", GRID_DEBUG);
	std::string line;
	std::smatch matches{};
	while (in)
	{
		std::getline(in, line);
		if (std::regex_match(line, m_GridRegex) && in)
		{
			std::regex_search(line, matches, m_GridRegex);
			if (matches[1] == "AmountOfLayers:")
				amountOfLayers = ReadSize_t(matches[2]);
			if (matches[1] == "Width:")
				width = ReadFloat(matches[2]);
			if (matches[1] == "Height:")
				height = ReadFloat(matches[2]);
			if (matches[1] == "BlockTexturePath:")
				blockTexturePaths.push_back(ReadTexturePath(matches[2]));
			if (matches[1] == "RevertOverIncrement:")
				revertOverIncrement = ReadBool(matches[2]);
			if (matches[1] == "Disk:")
				diskPositions.push_back(ReadDisk(matches[2]));
		}
	}

	if(amountOfLayers <= 0)
		engine::DebugManager::GetInstance().print("AmountOfLayers: none given or amountOfLayers equal to 0!", GRID_DEBUG);
	else if(width <= 0.f)
		engine::DebugManager::GetInstance().print("Width: none given or width smaller or equal to 0!", GRID_DEBUG);
	else if(height <= 0.f)
		engine::DebugManager::GetInstance().print("Height: none given or height smaller or equal to 0!", GRID_DEBUG);
	else if(blockTexturePaths.size() == 0 || !AreBlockTexturePathsValid(blockTexturePaths))
		engine::DebugManager::GetInstance().print("BlockTexturePath: no or invalid path given!", GRID_DEBUG);
	else if(diskPositions.size() == 0 || !AreDiskPositionsValid(diskPositions))
		engine::DebugManager::GetInstance().print("diskPositions: no or invalid date given!", GRID_DEBUG);
	else
	{
		engine::DebugManager::GetInstance().print("done", GRID_DEBUG);
		m_MaxLayers = amountOfLayers;
		GenerateLevel(scene, static_cast<size_t>(amountOfLayers), width, height, blockTexturePaths, revertOverIncrement, diskPositions);
	}
}

bool GridComponent::AreBlockTexturePathsValid(const std::vector<std::string>& blockTexturePaths)
{
	for(auto blockTexture : blockTexturePaths)
		if (blockTexture == "")
			return false;
	return true;
}

bool GridComponent::AreDiskPositionsValid(const std::vector<std::pair<int, bool>>& diskPositions)
{
	for (const auto diskPosition : diskPositions)
		if (diskPosition.first < 0)
			return false;
	return true;
}

void GridComponent::GenerateLevel(engine::Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, const std::vector<std::pair<int, bool>>& discPositions)
{
	engine::DebugManager::GetInstance().print("creating top node", GRID_DEBUG);
	const auto tempPos = m_pOwner.lock()->GetPosition();
	const auto tempNode = AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement, std::shared_ptr<GridNodeComponent>(nullptr), std::shared_ptr<GridNodeComponent>(nullptr));
	m_pSoloStartNode = tempNode;
	if (IsDiscNeeded(discPositions, m_MaxLayers - amountOfLayers, true))
		tempNode.lock()->SetDiscs({ AddDisc(scene, {tempPos.x - (width / 2.f), tempPos.y - height }, m_pSoloStartNode), std::shared_ptr<DiscComponent>(nullptr) });
	if (IsDiscNeeded(discPositions, m_MaxLayers - amountOfLayers, false))
		tempNode.lock()->SetDiscs({ AddDisc(scene, {tempPos.x + (width / 2.f), tempPos.y - height }, m_pSoloStartNode), std::shared_ptr<DiscComponent>(nullptr) });
	GenerateLayer(scene, --amountOfLayers, width, height, blockTexturePaths, revertOverIncrement, discPositions, { tempNode });
}

void GridComponent::GenerateLayer(engine::Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, const std::vector<std::pair<int, bool>>& discPositions, const std::vector<std::weak_ptr<GridNodeComponent>>& pPreviousLayer)
{
	m_pCoopStartNodes.first = pPreviousLayer[0];
	m_pCoopStartNodes.second = pPreviousLayer[pPreviousLayer.size() - 1];
	
	if(amountOfLayers < 1)
		return;

	std::vector<std::weak_ptr<GridNodeComponent>> pThisLayer;
	for (size_t j{ 0 }; j < pPreviousLayer.size(); ++j)
	{
		if (j == 0)
		{
			auto tempPos = pPreviousLayer[j].lock()->GetOwner().lock()->GetPosition();
			tempPos.x -= (width / 2.f);
			tempPos.y += height;
			engine::DebugManager::GetInstance().print("creating front node", GRID_DEBUG);
			auto node = AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement, std::shared_ptr<GridNodeComponent>(nullptr), pPreviousLayer[j]);
			if (IsDiscNeeded(discPositions, m_MaxLayers - amountOfLayers, true))
				node.lock()->SetDiscs({ AddDisc(scene, {tempPos.x - (width / 2.f), tempPos.y - height }, m_pSoloStartNode), std::shared_ptr<DiscComponent>(nullptr) });
			pThisLayer.push_back(node);
		}

		auto tempPos = pPreviousLayer[j].lock()->GetOwner().lock()->GetPosition();
		tempPos.x += (width / 2.f);
		tempPos.y += height;

		if (j < pPreviousLayer.size() - 1)
		{
			engine::DebugManager::GetInstance().print("creating mid node", GRID_DEBUG);
			pThisLayer.push_back(AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement, pPreviousLayer[j], pPreviousLayer[j + 1]));
		}
		else
		{
			engine::DebugManager::GetInstance().print("creating end node", GRID_DEBUG);
			auto node = AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement, pPreviousLayer[j], std::shared_ptr<GridNodeComponent>(nullptr));
			if (IsDiscNeeded(discPositions, m_MaxLayers - amountOfLayers, false))
				node.lock()->SetDiscs({ AddDisc(scene, {tempPos.x + (width / 2.f), tempPos.y - height }, m_pSoloStartNode), std::shared_ptr<DiscComponent>(nullptr) });
			pThisLayer.push_back(node);
		}
		
		if (j > 0)
			pPreviousLayer[j].lock()->SetConnection(engine::Direction::LEFT, pPreviousLayer[j], pPreviousLayer[j - 1]);
	}
	
	GenerateLayer(scene, --amountOfLayers, width, height, blockTexturePaths, revertOverIncrement, discPositions, pThisLayer);
}

std::weak_ptr<GridNodeComponent> GridComponent::AddNode(engine::Scene* scene, engine::Float2 pos, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, std::weak_ptr<GridNodeComponent> m_pTopLeftConnection, std::weak_ptr<GridNodeComponent> m_pTopRightConnection)
{
	auto obj = std::make_shared<engine::GameObject>();
	scene->Add(obj);
	auto pGnc = std::make_shared<GridNodeComponent>(obj, revertOverIncrement, blockTexturePaths);
	obj->AddComponent<GridNodeComponent>(pGnc);
	m_pGrid.push_back(pGnc);
	obj->SetPosition(pos);
	
	if(!m_pTopLeftConnection.expired())
		pGnc->SetConnection(engine::Direction::TOPLEFT, pGnc, m_pTopLeftConnection);

	if (!m_pTopRightConnection.expired())
		pGnc->SetConnection(engine::Direction::TOPRIGHT, pGnc, m_pTopRightConnection);

	engine::DebugManager::GetInstance().print("Node Created with pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y), NODE_DEBUG);
	
	return pGnc;
}

std::weak_ptr<DiscComponent> GridComponent::AddDisc(engine::Scene* scene, engine::Float2 pos, std::weak_ptr<GridNodeComponent> m_pTopNode) const
{
	auto obj = std::make_shared<engine::GameObject>();
	scene->Add(obj);
	auto pDisc = std::make_shared<DiscComponent>(obj, m_pTopNode);
	obj->AddComponent<DiscComponent>(pDisc);
	obj->SetPosition(pos);

	engine::DebugManager::GetInstance().print("Disc Created with pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y), NODE_DEBUG);

	return pDisc;
}

bool GridComponent::IsDiscNeeded(const std::vector<std::pair<int, bool>>& diskPositions, size_t layer, bool needsLeft)
{
	for(auto diskPos : diskPositions)
		if(layer == diskPos.first && diskPos.second == needsLeft)
			return true;
	return false;
}

size_t GridComponent::ReadSize_t(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_Size_tRegex))
	{
		std::regex_search(input, matches, m_Size_tRegex);
		return std::stoi(matches[1]);
	}
	return 0;
}

float GridComponent::ReadFloat(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_FloatRegex))
	{
		std::regex_search(input, matches, m_FloatRegex);
		return std::stof(matches[1]);
	}
	return 0;
}

std::string GridComponent::ReadTexturePath(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_TexturePathRegex))
	{
		std::regex_search(input, matches, m_TexturePathRegex);
		return matches[1];
	}
	return "";
}

bool GridComponent::ReadBool(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_BoolRegex))
	{
		std::regex_search(input, matches, m_BoolRegex);
		return (matches[1] == "true");
	}
	return false;
}

std::pair<int, bool> GridComponent::ReadDisk(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_DiskRegex))
	{
		std::regex_search(input, matches, m_DiskRegex);
		auto tempIndex = std::stoi(matches[1]);
		if (tempIndex >= 0 && (matches[2] == "left"))
			return std::pair<int, bool>{tempIndex, true};
		if (tempIndex >= 0 && (matches[2] == "right"))
			return std::pair<int, bool>{tempIndex, false};
	}
	return std::pair<int,bool>{-1, false};
}