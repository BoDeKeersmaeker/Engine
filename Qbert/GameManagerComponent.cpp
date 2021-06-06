#include "MiniginPCH.h"
#include "GameManagerComponent.h"

#include <algorithm>
#include <XInput.h>
#include "CoilyComponent.h"
#include "CounterComponent.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "GreenEnemyComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "Move.h"
#include "PlayerComponent.h"
#include "PurpleEnemyComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ScoreObserver.h"
#include "SubjectComponent.h"

GameManagerComponent::GameManagerComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<engine::Scene> scene, GameMode gameMode, const std::vector<std::string>& levelPaths)
	:Component(owner)
	, m_LevelPaths( levelPaths )
	, m_pScene( scene )
	, m_GameMode( gameMode )
	, m_pPlayers{ std::shared_ptr<PlayerComponent>(nullptr), std::shared_ptr<PlayerComponent>(nullptr) }
{
	auto scoreCounter = std::make_shared<engine::GameObject>();
	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	scoreCounter->AddComponent<engine::CounterComponent>(std::make_shared<engine::CounterComponent>(scoreCounter, font, 0, "Score: "));
	scoreCounter->SetPosition(80.f, 130.f);
	m_pScene.lock()->Add(scoreCounter);

	m_pScoreObserver = std::make_shared<ScoreObserver>(scoreCounter->GetComponent<engine::CounterComponent>());

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

	SetEnemyCooldown();
}

void GameManagerComponent::Update()
{
	CleanUppEnemies();
	CheckCharacteOverlap();

	if (m_ChangingLevel)
	{
		NextLevel();
		m_ChangingLevel = false;
	}
	
	auto elapsedSec = engine::EngineTime::GetInstance().GetElapsedSec();
	
	if(m_CurrentGreenEnemyCooldown >= 0.f)
		m_CurrentGreenEnemyCooldown -= elapsedSec;
	if (m_CurrentGreenEnemyCooldown <= 0.f)
		SpawnGreenEnemy();

	if (m_CurrentPurpleEnemyCooldown >= 0.f)
		m_CurrentPurpleEnemyCooldown -= elapsedSec;
	if (m_CurrentPurpleEnemyCooldown <= 0.f)
		SpawnPurpleEnemy();

	if (m_GameMode != GameMode::Versus)
	{
		if (m_CurrentCoilyCooldown >= 0.f)
			m_CurrentCoilyCooldown -= elapsedSec;
		if (m_CurrentCoilyCooldown <= 0.f && m_pCoily.expired())
			SpawnCoily();
	}

	if (m_pGrid.lock()->IsCompleted())
	{
		ClearEnemies();
		m_ChangingLevel = true;
	}
}

void GameManagerComponent::Render(const engine::Transform&)
{

}

void GameManagerComponent::SetEnemyCooldown(float greenEnemyCooldown, float purpleEnemyCooldown, float coilyCooldown)
{
	m_GreenEnemyCooldown = greenEnemyCooldown;
	m_CurrentGreenEnemyCooldown = greenEnemyCooldown;
	m_PurpleEnemyCooldown = purpleEnemyCooldown;
	m_CurrentPurpleEnemyCooldown = purpleEnemyCooldown;
	m_CoilyCooldown = coilyCooldown;
	m_CurrentCoilyCooldown = coilyCooldown;
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

int GameManagerComponent::GetScore() const
{
	return m_Score;
}

void GameManagerComponent::LoadLevel(size_t index)
{
	if (!m_pGrid.expired())
	{
		m_pGrid.lock()->Clear();
		m_pGrid.lock()->GetOwner().lock()->Destroy();
	}
	
	auto obj = std::make_shared<engine::GameObject>();
	obj->SetPosition(m_pOwner.lock()->GetPosition());
	obj->AddComponent<GridComponent>(std::make_shared<GridComponent>(obj, m_pScene, m_LevelPaths[index], 1, m_pScoreObserver));
	m_pGrid = obj->GetComponent<GridComponent>();
	m_pScene.lock()->Add(obj, 1);
	
	switch (m_GameMode)
	{
		case GameMode::Single:
			if (!m_pPlayers.first.expired())
				m_pPlayers.first.lock()->Reset(m_pGrid.lock()->GetSoloStartNode());
			break;
		case GameMode::Coop:
			if (!m_pPlayers.first.expired())
				m_pPlayers.first.lock()->Reset(m_pGrid.lock()->GetSoloStartNode());
			if (!m_pPlayers.second.expired())
				m_pPlayers.second.lock()->Reset(m_pGrid.lock()->GetSoloStartNode());
			break;
		case GameMode::Versus:
			if (!m_pPlayers.first.expired())
				m_pPlayers.first.lock()->Reset(m_pGrid.lock()->GetSoloStartNode());
			break;
	}
}

void GameManagerComponent::LoadSingle()
{	
	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetSoloStartNode()));
	//obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver();
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
	std::cout << "To be implemented!\n";
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
	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(m_pScoreObserver);
	m_pScene.lock()->Add(obj);
	m_pGreenEnemies.push_back(obj->GetComponent<GreenEnemyComponent>());
	
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
	m_pPurpleEnemies.push_back(obj->GetComponent<PurpleEnemyComponent>());
	
	engine::DebugManager::GetInstance().print("Purple enemy spawned.", GAMEMANAGER_DEBUG);
}

void GameManagerComponent::SpawnCoily()
{
	m_CurrentCoilyCooldown = m_CoilyCooldown;

	if (m_GameMode == GameMode::Single)
	{
		auto obj = std::make_shared<engine::GameObject>();
		obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
		m_pCoily = obj->GetComponent<CoilyComponent>();
		m_pScene.lock()->Add(obj);
	}
	else if(m_GameMode == GameMode::Coop)
	{
		auto obj = std::make_shared<engine::GameObject>();
		if (rand() % 2)
			obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
		else
			obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.second));
		m_pCoily = obj->GetComponent<CoilyComponent>();
		m_pScene.lock()->Add(obj);
	}
	
	engine::DebugManager::GetInstance().print("Coily spawned.", GAMEMANAGER_DEBUG);
}

void GameManagerComponent::ClearEnemies()
{
	for (auto comp : m_pGreenEnemies)
		if(!comp.expired())
			comp.lock()->GetOwner().lock()->Destroy();

	for (auto comp : m_pPurpleEnemies)
		if(!comp.expired())
			comp.lock()->GetOwner().lock()->Destroy();
	
	if(!m_pCoily.expired())
		m_pCoily.lock()->GetOwner().lock()->Destroy();
}

void GameManagerComponent::CleanUppEnemies()
{
	m_pGreenEnemies.erase(std::remove_if(m_pGreenEnemies.begin(), m_pGreenEnemies.end(), [](const std::weak_ptr<GreenEnemyComponent>& comp) { return comp.expired(); }), m_pGreenEnemies.end());
	m_pPurpleEnemies.erase(std::remove_if(m_pPurpleEnemies.begin(), m_pPurpleEnemies.end(), [](const std::weak_ptr<PurpleEnemyComponent>& comp) { return comp.expired(); }), m_pPurpleEnemies.end());
}

void GameManagerComponent::CheckCharacteOverlap()
{
	for (auto comp : m_pGreenEnemies)
	{
		if (!comp.expired())
		{
			if (!m_pPlayers.first.expired() && comp.lock()->CheckOverlap(m_pPlayers.first.lock()->GetCurrentNode()))
				comp.lock()->GetOwner().lock()->Destroy();
			if (!m_pPlayers.second.expired() && comp.lock()->CheckOverlap(m_pPlayers.second.lock()->GetCurrentNode()))
				comp.lock()->GetOwner().lock()->Destroy();
		}
	}
	
	for (auto comp : m_pPurpleEnemies)
	{
		if (!comp.expired())
		{
			if (!m_pPlayers.first.expired() && comp.lock()->CheckOverlap(m_pPlayers.first.lock()->GetCurrentNode()))
				m_pPlayers.first.lock()->Die();
			if (!m_pPlayers.second.expired() && comp.lock()->CheckOverlap(m_pPlayers.second.lock()->GetCurrentNode()))
				m_pPlayers.second.lock()->Die();
		}
	}

	if(!m_pCoily.expired())
	{
		if(!m_pPlayers.first.expired() && !m_pPlayers.first.lock()->IsOnDisk() && m_pCoily.lock()->GetCurrentNode().lock() == m_pPlayers.first.lock()->GetCurrentNode().lock())
			m_pPlayers.first.lock()->Die();
		if (!m_pPlayers.second.expired() && !m_pPlayers.second.lock()->IsOnDisk() && m_pCoily.lock()->GetCurrentNode().lock() == m_pPlayers.second.lock()->GetCurrentNode().lock())
			m_pPlayers.second.lock()->Die();
	}
}