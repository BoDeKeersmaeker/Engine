#include "MiniginPCH.h"
#include "GridNodeComponent.h"

#include "DebugManager.h"
#include "GameObject.h"
#include "RenderComponent.h"

engine::GridNodeComponent::GridNodeComponent(std::shared_ptr<GameObject> owner, bool revertOverIncrement, const std::vector<std::string>& blockPaths)
	:Component(owner)
	, m_pConnections{
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr),
		std::shared_ptr<GridNodeComponent>(nullptr) }
	, m_BlockPaths{ blockPaths }
	, m_RevertOverIncrement{ revertOverIncrement }
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

std::weak_ptr<engine::GridNodeComponent> engine::GridNodeComponent::GetConnection(ConnectionDirection connectionDirection)
{
	return m_pConnections[static_cast<size_t>(connectionDirection)];
}

void engine::GridNodeComponent::SetConnection(ConnectionDirection connectionDirection, std::weak_ptr<GridNodeComponent> pThisNode, std::weak_ptr<GridNodeComponent> pOtherNode)
{
	m_pConnections[static_cast<size_t>(connectionDirection)] = pOtherNode;
	
	auto tempNode = pOtherNode.lock();

	size_t tempDir = static_cast<size_t>(connectionDirection) + 2;
	
	if (tempDir >= 4)
		tempDir -= 4;
	
	if (tempNode->GetConnection(static_cast<ConnectionDirection>(tempDir)).expired())
	{
		DebugManager::GetInstance().print("Making return connection", NODE_DEBUG);
		tempNode->SetConnection(static_cast<ConnectionDirection>(tempDir), pOtherNode, pThisNode);
	}

	DebugManager::GetInstance().print("Node: " + std::to_string(static_cast<int>(connectionDirection)) + " connected.", NODE_DEBUG);
}

void engine::GridNodeComponent::Increment()
{
	size_t oldIndex = m_CurrentBlockIndex;

	if (m_CurrentBlockIndex + 1 < m_BlockPaths.size())
	{
		++m_CurrentBlockIndex;
		DebugManager::GetInstance().print("Node incremented", NODE_DEBUG);
	}
	else if(m_RevertOverIncrement)
		Decrement();

	if (oldIndex != m_CurrentBlockIndex)
		m_pRenderComponent.lock()->SetTexture(m_BlockPaths[m_CurrentBlockIndex]);
}

void engine::GridNodeComponent::Decrement()
{
	size_t oldIndex = m_CurrentBlockIndex;
	
	if (m_CurrentBlockIndex != 0)
	{
		--m_CurrentBlockIndex;
		DebugManager::GetInstance().print("Node decremented", NODE_DEBUG);
	}
	
	if (oldIndex != m_CurrentBlockIndex)
		m_pRenderComponent.lock()->SetTexture(m_BlockPaths[m_CurrentBlockIndex]);
}

bool engine::GridNodeComponent::IsCompleted() const
{
	return m_CurrentBlockIndex == m_BlockPaths.size() - 1;
}