#include "MiniginPCH.h"
#include "GameManagerComponent.h"

#include <algorithm>
#include <XInput.h>
#include "CoilyComponent.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "GreenEnemyComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "Move.h"
#include "PlayerComponent.h"
#include "PurpleEnemyComponent.h"
#include "RenderComponent.h"
#include "Scene.h"

GameManagerComponent::GameManagerComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<engine::Scene> scene, GameMode gameMode, const std::vector<std::string>& levelPaths)
	:Component(owner)
	, m_LevelPaths( levelPaths )
	, m_pScene( scene )
	, m_GameMode( gameMode )
	, m_pPlayers{ std::shared_ptr<PlayerComponent>(nullptr), std::shared_ptr<PlayerComponent>(nullptr) }
{
	LoadLevel(0);

	switch (gameMode)
	{
		case GameMode::Single:
			LoadSingle();
			break;
		case GameMode::Coop:
			LoadCoop();
			break;
		case GameMode::Versus:
			LoadVersus();
			break;
	}
}

void GameManagerComponent::Update()
{
	//auto elapsedSec = engine::EngineTime::GetInstance().GetElapsedSec();
	
	//if(m_CurrentGreenEnemyCooldown >= 0.f)
	//	m_CurrentGreenEnemyCooldown -= elapsedSec;
	//if (m_CurrentGreenEnemyCooldown <= 0.f)
	//	SpawnGreenEnemy();

	//if (m_CurrentPurpleEnemyCooldown >= 0.f)
	//	m_CurrentPurpleEnemyCooldown -= elapsedSec;
	//if (m_CurrentPurpleEnemyCooldown <= 0.f)
	//	SpawnPurpleEnemy();

	//if (m_GameMode != GameMode::Versus)
	//{
	//	if (m_CurrentCoilyCooldown >= 0.f)
	//		m_CurrentCoilyCooldown -= elapsedSec;
	//	if (m_CurrentCoilyCooldown <= 0.f)
	//		SpawnCoily();
	//}

	if (m_pGrid.lock()->IsCompleted())
		NextLevel();
}

void GameManagerComponent::Render(const engine::Transform&)
{

}

void GameManagerComponent::SetEnemyCooldown(float greenEnemyCooldown, float purpleEnemyCooldown, float coilyCooldown)
{
	m_GreenEnemyCooldown = greenEnemyCooldown;
	m_PurpleEnemyCooldown = purpleEnemyCooldown;
	m_CoilyCooldown = coilyCooldown;
}

void GameManagerComponent::NextLevel()
{
	if(m_CurrentLevelIndex + 1 < m_LevelPaths.size())
	{
		m_CurrentLevelIndex++;
		LoadLevel(m_CurrentLevelIndex);
	}
	else
		m_GameWon = true;
}

bool GameManagerComponent::isGameWon() const
{
	return m_GameWon;
}

void GameManagerComponent::LoadLevel(size_t index)
{	
	auto obj = std::make_shared<engine::GameObject>();
	obj->SetPosition(m_pOwner.lock()->GetPosition());
	obj->AddComponent<GridComponent>(std::make_shared<GridComponent>(obj, m_pScene.lock().get(), m_LevelPaths[index], 1));

	ClearEnemies();

	if (!m_pGrid.expired())
	{
		m_pGrid.lock()->Clear();
		m_pGrid.lock()->GetOwner().lock()->Destroy();
	}
	
	m_pGrid = obj->GetComponent<GridComponent>();
	m_pScene.lock()->Add(obj, 1);
	
	switch (m_GameMode)
	{
		case GameMode::Single:
			if (!m_pPlayers.first.expired())
			{
				m_pPlayers.first.lock()->SetSpawnNode(m_pGrid.lock()->GetSoloStartNode());
				m_pPlayers.first.lock()->SetCurrentNode(m_pGrid.lock()->GetSoloStartNode());
			}
			break;
		case GameMode::Coop:
			if (!m_pPlayers.first.expired())
			{
				m_pPlayers.first.lock()->SetSpawnNode(m_pGrid.lock()->GetSoloStartNode());
				m_pPlayers.first.lock()->SetCurrentNode(m_pGrid.lock()->GetSoloStartNode());
			}
			if (!m_pPlayers.second.expired())
			{
				m_pPlayers.second.lock()->SetSpawnNode(m_pGrid.lock()->GetSoloStartNode());
				m_pPlayers.second.lock()->SetCurrentNode(m_pGrid.lock()->GetSoloStartNode());
			}
			break;
		case GameMode::Versus:
			if (!m_pPlayers.first.expired())
			{
				m_pPlayers.first.lock()->SetSpawnNode(m_pGrid.lock()->GetSoloStartNode());
				m_pPlayers.first.lock()->SetCurrentNode(m_pGrid.lock()->GetSoloStartNode());
			}
			break;
	}
}

void GameManagerComponent::LoadSingle()
{
	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetSoloStartNode()));
	m_pPlayers.first = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj, 0);

	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddCommand(VK_PAD_Y, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_B, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_A, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_X, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddCommand('A', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand('Z', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand('S', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand('Q', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));
}

void GameManagerComponent::LoadCoop()
{
	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetCoopStartNodes().first));
	m_pPlayers.first = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj);

	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_LTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddCommand(VK_PAD_Y, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_B, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_A, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand(VK_PAD_X, engine::InputTriggerType::OnInputDown, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddCommand('A', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand('Z', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand('S', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand('Q', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetCoopStartNodes().second));
	m_pPlayers.second = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj);

	engine::InputManager::GetInstance().AddCommand('O', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.second, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddCommand('P', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.second, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddCommand('M', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.second, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddCommand('L', engine::InputTriggerType::OnInputHold, std::make_shared<Move>(m_pPlayers.second, engine::Direction::BOTTOMLEFT));
}

void GameManagerComponent::LoadVersus()
{
	
}

void GameManagerComponent::SpawnGreenEnemy()
{
	m_CurrentGreenEnemyCooldown = m_GreenEnemyCooldown;

	auto obj = std::make_shared<engine::GameObject>();
	
	if(rand() % 2)
		obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Slick.png", engine::Float2{ 0.f, -32.f }));
	else
		obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Sam.png", engine::Float2{ 0.f, -32.f }));

	obj->AddComponent<GreenEnemyComponent>(std::make_shared<GreenEnemyComponent>(obj, m_pGrid.lock()->GetSoloStartNode()));
	m_pScene.lock()->Add(obj);

	m_pEnemies.push_back(obj);
	
	engine::DebugManager::GetInstance().print("Green enemy spawned.", GAMEMANAGER_DEBUG);
}

void GameManagerComponent::SpawnPurpleEnemy()
{
	m_CurrentPurpleEnemyCooldown = m_PurpleEnemyCooldown;

	auto obj = std::make_shared<engine::GameObject>();

	if (rand() % 2)
	{
		obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "WrongWay.png", engine::Float2{ -24.f, 15.f }));
		obj->AddComponent<PurpleEnemyComponent>(std::make_shared<PurpleEnemyComponent>(obj, false, m_pGrid.lock()->GetCoopStartNodes().first));
	}
	else
	{
		obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Ugg.png", engine::Float2{ 24.f, 15.f }));
		obj->AddComponent<PurpleEnemyComponent>(std::make_shared<PurpleEnemyComponent>(obj, true, m_pGrid.lock()->GetCoopStartNodes().second));
	}
	m_pScene.lock()->Add(obj);
	
	engine::DebugManager::GetInstance().print("Purple enemy spawned.", GAMEMANAGER_DEBUG);
}

void GameManagerComponent::SpawnCoily()
{
	m_CurrentCoilyCooldown = m_CoilyCooldown;

	if (m_GameMode == GameMode::Single)
	{
		auto obj = std::make_shared<engine::GameObject>();
		obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
		m_pScene.lock()->Add(obj);
	}
	else if(m_GameMode == GameMode::Coop)
	{
		auto obj = std::make_shared<engine::GameObject>();
		if (rand() % 2)
			obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
		else
			obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.second));
		m_pScene.lock()->Add(obj);
	}
	
	engine::DebugManager::GetInstance().print("Coily spawned.", GAMEMANAGER_DEBUG);
}

void GameManagerComponent::ClearEnemies()
{
	m_pEnemies.erase(std::remove_if(m_pEnemies.begin(), m_pEnemies.end(), []( std::weak_ptr<engine::GameObject> enemy )
	{
		if(!enemy.expired())
			enemy.lock()->Destroy();
		return true;
	}), m_pEnemies.end()); 
}