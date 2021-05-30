#include "MiniginPCH.h"
#include "GridComponent.h"
#include "GameObject.h"
#include "GridNodeComponent.h"
#include "Scene.h"

engine::GridComponent::GridComponent(std::shared_ptr<GameObject> owner, const std::vector<std::string>& blockPaths)
	:Component(owner)
	, m_BlockPaths{ blockPaths }
{
	
}

void engine::GridComponent::Update()
{

}

void engine::GridComponent::Render(const Transform&)
{

}

void engine::GridComponent::GenerateGrid(Scene* scene, UINT amountOfLayers, float size)
{
	auto tempNode = AddNode(scene, m_pOwner.lock()->GetPosition(), std::shared_ptr<GridNodeComponent>(nullptr), std::shared_ptr<GridNodeComponent>(nullptr));
	GenerateLayer(scene, --amountOfLayers, size, { tempNode });
}

void engine::GridComponent::GenerateLayer(Scene* scene, UINT amountOfLayers, float size, const std::vector<std::weak_ptr<GridNodeComponent>>& pPreviousLayer)
{
	if(amountOfLayers < 1)
		return;
	
	std::vector<std::weak_ptr<GridNodeComponent>> pThisLayer;
	for (size_t j{}; j < pPreviousLayer.size(); ++j)
	{
		if (j == 0)
		{
			auto tempPos = pPreviousLayer[j].lock()->GetOwner().lock()->GetPosition();
			tempPos.x -= (size / 2.f) + 3.f;
			tempPos.y += size;
			pThisLayer.push_back(AddNode(scene, tempPos, std::shared_ptr<GridNodeComponent>(nullptr), pPreviousLayer[j]));
			DebugManager::GetInstance().print("creating front node", 2);
		}

		auto tempPos = pPreviousLayer[j].lock()->GetOwner().lock()->GetPosition();
		tempPos.x += (size / 2.f) + 3.f;
		tempPos.y -= size;

		if (j < pPreviousLayer.size() - 1)
		{
			pThisLayer.push_back(AddNode(scene, tempPos, pPreviousLayer[j], pPreviousLayer[j + 1]));
			DebugManager::GetInstance().print("creating mid node", 2);
		}
		else
		{
			DebugManager::GetInstance().print("creating end node", 2);
			pThisLayer.push_back(AddNode(scene, tempPos, pPreviousLayer[j], std::shared_ptr<GridNodeComponent>(nullptr)));
		}
	}

	GenerateLayer(scene, --amountOfLayers, size, pThisLayer);
}

std::weak_ptr<engine::GridNodeComponent> engine::GridComponent::AddNode(Scene* scene, Float2 pos, std::weak_ptr<GridNodeComponent> m_pTopLeftConnection, std::weak_ptr<GridNodeComponent> m_pTopRightConnection)
{
	auto obj = std::make_shared<engine::GameObject>();
	auto pGnc = std::make_shared<engine::GridNodeComponent>(obj, m_BlockPaths);
	
	if(m_pTopLeftConnection.lock() != std::shared_ptr<GridNodeComponent>( nullptr))
		pGnc->SetConnection(ConnectionDirection::TOPLEFT, m_pTopLeftConnection);

	if (m_pTopRightConnection.lock() != std::shared_ptr<GridNodeComponent>(nullptr))
		pGnc->SetConnection(ConnectionDirection::TOPRIGHT, m_pTopRightConnection);
	
	m_pGrid.push_back(pGnc);
	obj->AddComponent<GridNodeComponent>(pGnc);
	obj->SetPosition(pos);
	scene->Add(obj);
	DebugManager::GetInstance().print("Node Created with pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y), NODE_DEBUG);
	return pGnc;
}