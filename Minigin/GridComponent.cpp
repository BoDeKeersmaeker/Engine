#include "MiniginPCH.h"
#include "GridComponent.h"

#include <fstream>

#include "DebugManager.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "Scene.h"

engine::GridComponent::GridComponent(std::shared_ptr<GameObject> owner, Scene* scene, const std::string& filePath)
	:Component(owner)
	, m_pGrid{}
{
	ReadLevelFile(scene, filePath);
}

void engine::GridComponent::Update()
{

}

void engine::GridComponent::Render(const Transform&)
{

}

void engine::GridComponent::ReadLevelFile(Scene* scene, const std::string& filePath)
{
	std::ifstream in{ filePath };
	if (!in)
	{
		DebugManager::GetInstance().print("Level file not opened for reading.", GRID_DEBUG);
		return;
	}

	float amountOfLayers = 0;
	float width = 0;
	float height = 0;
	std::vector<std::string> blockTexturePaths;
	bool revertOverIncrement = false;
	
	DebugManager::GetInstance().print("reading from Level file: ", GRID_DEBUG);
	std::string line;
	std::smatch matches{};
	while (in)
	{
		std::getline(in, line);
		if (std::regex_match(line, m_GridRegex) && in)
		{
			std::regex_search(line, matches, m_GridRegex);
			if (matches[1] == "AmountOfLayers:")
				amountOfLayers = ReadFloat(matches[2]);
			if (matches[1] == "Width:")
				width = ReadFloat(matches[2]);
			if (matches[1] == "Height:")
				height = ReadFloat(matches[2]);
			if (matches[1] == "BlockTexturePath:")
				blockTexturePaths.push_back(ReadTexturePath(matches[2]));
			if (matches[1] == "revertOverIncrement:")
				revertOverIncrement = ReadBool(matches[2]);
		}
	}

	if(amountOfLayers <= 0)
		DebugManager::GetInstance().print("No AmountOfLayers: given or AmountOfLayers: equal to 0!", GRID_DEBUG);
	else if(width <= 0.f)
		DebugManager::GetInstance().print("No Width: given or Width: smaller or equal to 0!", GRID_DEBUG);
	else if(height <= 0.f)
		DebugManager::GetInstance().print("No Height: given or Height: smaller or equal to 0!", GRID_DEBUG);
	else if(blockTexturePaths.size() == 0 && !IsBlockTexturePathsValid(blockTexturePaths))
		DebugManager::GetInstance().print("No BlockTexturePath: given!", GRID_DEBUG);
	else
	{
		DebugManager::GetInstance().print("done", GRID_DEBUG);
		GenerateLevel(scene, static_cast<size_t>(amountOfLayers), width, height, blockTexturePaths, revertOverIncrement);
	}
}

bool engine::GridComponent::IsBlockTexturePathsValid(const std::vector<std::string>& blockTexturePaths)
{
	for(auto blockTexture : blockTexturePaths)
		if (blockTexture == "")
			return false;
	return true;
}

void engine::GridComponent::GenerateLevel(Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement)
{
	DebugManager::GetInstance().print("creating top node", GRID_DEBUG);
	const auto tempNode = AddNode(scene, m_pOwner.lock()->GetPosition(), blockTexturePaths, revertOverIncrement, std::shared_ptr<GridNodeComponent>(nullptr), std::shared_ptr<GridNodeComponent>(nullptr));
	m_pSoloStartNode = tempNode;
	GenerateLayer(scene, --amountOfLayers, width, height, blockTexturePaths, revertOverIncrement, { tempNode });
}

void engine::GridComponent::GenerateLayer(Scene* scene, size_t amountOfLayers, float width, float height, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, const std::vector<std::weak_ptr<GridNodeComponent>>& pPreviousLayer)
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
			DebugManager::GetInstance().print("creating front node", GRID_DEBUG);
			pThisLayer.push_back(AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement,std::shared_ptr<GridNodeComponent>(nullptr),  pPreviousLayer[j]));
		}

		auto tempPos = pPreviousLayer[j].lock()->GetOwner().lock()->GetPosition();
		tempPos.x += (width / 2.f);
		tempPos.y += height;

		if (j < pPreviousLayer.size() - 1)
		{
			DebugManager::GetInstance().print("creating mid node", GRID_DEBUG);
			pThisLayer.push_back(AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement, pPreviousLayer[j], pPreviousLayer[j + 1]));
		}
		else
		{
			DebugManager::GetInstance().print("creating end node", GRID_DEBUG);
			pThisLayer.push_back(AddNode(scene, tempPos, blockTexturePaths, revertOverIncrement, pPreviousLayer[j], std::shared_ptr<GridNodeComponent>(nullptr)));
		}
		
		if (j > 0)
			pPreviousLayer[j].lock()->SetConnection(Direction::LEFT, pPreviousLayer[j], pPreviousLayer[j - 1]);
	}
	
	GenerateLayer(scene, --amountOfLayers, width, height, blockTexturePaths, revertOverIncrement, pThisLayer);
}

std::weak_ptr<engine::GridNodeComponent> engine::GridComponent::AddNode(Scene* scene, Float2 pos, const std::vector<std::string>& blockTexturePaths, bool revertOverIncrement, std::weak_ptr<GridNodeComponent> m_pTopLeftConnection, std::weak_ptr<GridNodeComponent> m_pTopRightConnection)
{
	auto obj = std::make_shared<engine::GameObject>();
	scene->Add(obj);
	auto pGnc = std::make_shared<engine::GridNodeComponent>(obj, revertOverIncrement, blockTexturePaths);
	obj->AddComponent<GridNodeComponent>(pGnc);
	m_pGrid.push_back(pGnc);
	obj->SetPosition(pos);
	
	if(!m_pTopLeftConnection.expired())
		pGnc->SetConnection(Direction::TOPLEFT, pGnc, m_pTopLeftConnection);

	if (!m_pTopRightConnection.expired())
		pGnc->SetConnection(Direction::TOPRIGHT, pGnc, m_pTopRightConnection);
	
	DebugManager::GetInstance().print("Node Created with pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y), NODE_DEBUG);
	
	return obj->GetComponent<engine::GridNodeComponent>().lock();
}

size_t engine::GridComponent::ReadSize_t(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_Size_tRegex))
	{
		std::regex_search(input, matches, m_Size_tRegex);
		return std::stoi(matches[1]);
	}
	return 0;
}

float engine::GridComponent::ReadFloat(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_FloatRegex))
	{
		std::regex_search(input, matches, m_FloatRegex);
		return std::stof(matches[1]);
	}
	return 0;
}

std::string engine::GridComponent::ReadTexturePath(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_TexturePathRegex))
	{
		std::regex_search(input, matches, m_TexturePathRegex);
		return matches[1];
	}
	return "";
}

bool engine::GridComponent::ReadBool(const std::string& input) const
{
	std::smatch matches{};
	if (regex_match(input, m_BoolRegex))
	{
		std::regex_search(input, matches, m_BoolRegex);
		return (matches[1] == "true");
	}
	return false;
}