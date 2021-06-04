#include "MiniginPCH.h"
#include "GridNodeComponent.h"

#include "DebugManager.h"
#include "GameObject.h"
#include "RenderComponent.h"

GridNodeComponent::GridNodeComponent(std::shared_ptr<engine::GameObject> owner, bool revertOverIncrement, const std::vector<std::string>& blockPaths)
	:Component(owner)
	, m_pConnections{
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr) }
	, m_pDiscs{
		std::shared_ptr<DiscComponent>(nullptr),
		std::shared_ptr<DiscComponent>(nullptr) }
	, m_BlockPaths{ blockPaths }
	, m_RevertOverIncrement{ revertOverIncrement }
{
	owner->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(owner, m_BlockPaths[0]));
	m_pRenderComponent = owner->GetComponent<engine::RenderComponent>();
	m_pRenderComponent.lock()->SetTexture(m_BlockPaths[0]);
}

void GridNodeComponent::Update()
{

}

void GridNodeComponent::Render(const engine::Transform&)
{

}

std::weak_ptr<GridNodeComponent> GridNodeComponent::GetConnection(engine::Direction connectionDirection)
{
	return m_pConnections[static_cast<size_t>(connectionDirection)];
}

void GridNodeComponent::SetConnection(engine::Direction connectionDirection, std::weak_ptr<GridNodeComponent> pThisNode, std::weak_ptr<GridNodeComponent> pOtherNode)
{
	m_pConnections[static_cast<size_t>(connectionDirection)] = pOtherNode;
	
	auto tempNode = pOtherNode.lock();

	size_t tempDir = static_cast<size_t>(connectionDirection) + 2;
	if (connectionDirection != engine::Direction::LEFT && connectionDirection != engine::Direction::RIGHT)
	{
		if (tempDir >= 4)
			tempDir -= 4;
	}
	else
	{
		switch (connectionDirection)
		{
			case engine::Direction::LEFT:
				tempDir = static_cast<size_t>(engine::Direction::RIGHT);
				break;
			case engine::Direction::RIGHT:
				tempDir = static_cast<size_t>(engine::Direction::LEFT);
				break;
			default:
				return;
		}
	}
	
	if (tempNode->GetConnection(static_cast<engine::Direction>(tempDir)).expired())
	{
		engine::DebugManager::GetInstance().print("Making return connection", NODE_DEBUG);
		tempNode->SetConnection(static_cast<engine::Direction>(tempDir), pOtherNode, pThisNode);
	}

	engine::DebugManager::GetInstance().print("Node: " + std::to_string(static_cast<int>(connectionDirection)) + " connected.", NODE_DEBUG);
}

void GridNodeComponent::SetDiscs(std::pair<std::weak_ptr<DiscComponent>, std::weak_ptr<DiscComponent>> pDiscs)
{
	m_pDiscs = pDiscs;
}

std::pair<std::weak_ptr<DiscComponent>, std::weak_ptr<DiscComponent>> GridNodeComponent::GetDiscs() const
{
	return m_pDiscs;
}

void GridNodeComponent::Increment()
{
	size_t oldIndex = m_CurrentBlockIndex;

	if (m_CurrentBlockIndex + 1 < m_BlockPaths.size())
	{
		++m_CurrentBlockIndex;
		engine::DebugManager::GetInstance().print("Node incremented", NODE_DEBUG);
	}
	else if(m_RevertOverIncrement)
		Decrement();

	if (oldIndex != m_CurrentBlockIndex)
		m_pRenderComponent.lock()->SetTexture(m_BlockPaths[m_CurrentBlockIndex]);
}

void GridNodeComponent::Decrement()
{
	size_t oldIndex = m_CurrentBlockIndex;
	
	if (m_CurrentBlockIndex != 0)
	{
		--m_CurrentBlockIndex;
		engine::DebugManager::GetInstance().print("Node decremented", NODE_DEBUG);
	}
	
	if (oldIndex != m_CurrentBlockIndex)
		m_pRenderComponent.lock()->SetTexture(m_BlockPaths[m_CurrentBlockIndex]);
}

bool GridNodeComponent::IsCompleted() const
{
	return m_CurrentBlockIndex == m_BlockPaths.size() - 1;
}