#include "MiniginPCH.h"
#include "GridNodeComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

engine::GridNodeComponent::GridNodeComponent(std::shared_ptr<GameObject> owner, const std::vector<std::string>& blockPaths)
	:Component(owner)
	, m_pConnections{
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr) ,
		std::shared_ptr<GridNodeComponent>(nullptr) ,
		std::shared_ptr<GridNodeComponent>(nullptr) }
	, m_BlockPaths{ blockPaths }
{
	owner->AddComponent<RenderComponent>(std::make_shared<engine::RenderComponent>(owner, m_BlockPaths[0]));
	m_pRenderComponent = owner->GetComponent<RenderComponent>();
	m_pRenderComponent.lock()->SetTexture(m_BlockPaths[0]);
}

void engine::GridNodeComponent::Update()
{

}

void engine::GridNodeComponent::Render(const Transform&)
{

}

const std::weak_ptr<engine::GridNodeComponent>& engine::GridNodeComponent::GetConnection(ConnectionDirection connectionDirection)
{
	return m_pConnections[static_cast<int>(connectionDirection)];
}

void engine::GridNodeComponent::SetConnection(ConnectionDirection connectionDirection, std::weak_ptr<GridNodeComponent> pOtherNode)
{
	m_pConnections[static_cast<size_t>(connectionDirection)] = pOtherNode;
	
	auto tempNode = pOtherNode.lock();

	size_t tempDir = static_cast<size_t>(connectionDirection) + 2;
	if (tempDir >= 4)
		tempDir -= 4;
	if (tempNode->GetConnection(static_cast<ConnectionDirection>(tempDir)).lock() == nullptr)
		tempNode->SetConnection(static_cast<ConnectionDirection>(tempDir), std::make_shared<GridNodeComponent>(*this));

	std::string tempText = "Node ";

	switch (connectionDirection)
	{
	case ConnectionDirection::TOPLEFT:
		tempText += "TOPLEFT";
		break;
	case ConnectionDirection::TOPRIGHT:
		tempText += "TOPRIGHT";
		break;
	case ConnectionDirection::BOTTOMLEFT:
		tempText += "BOTTOMLEFT";
		break;
	case ConnectionDirection::BOTTOMRIGHT:
		tempText += "BOTTOMRIGHT";
		break;
	}
	
	tempText += " Connected.";
	
	DebugManager::GetInstance().print(tempText, NODE_DEBUG);
}