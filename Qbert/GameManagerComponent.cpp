#include "MiniginPCH.h"
#include "GameManagerComponent.h"

#include <algorithm>
#include <XInput.h>
#include "CoilyComponent.h"
#include "CoilyMove.h"
#include "CounterComponent.h"
#include "EngineTime.h"
#include "GameObject.h"
#include "GreenEnemyComponent.h"
#include "GridComponent.h"
#include "InputManager.h"
#include "LifeObserver.h"
#include "PlayerMove.h"
#include "PlayerComponent.h"
#include "PurpleEnemyComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ScoreObserver.h"
#include "SubjectComponent.h"
#include "../3rdParty/SDL2/include/SDL_keycode.h"

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
	scoreCounter->SetPosition(m_pOwner.lock()->GetPosition().x, m_pOwner.lock()->GetPosition().y - 50.f);
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
}

void GameManagerComponent::Update()
{
	CleanUppEnemies();
	CheckCharacterOverlap();

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

	if (m_pCoily.expired() && m_CurrentCoilyCooldown >= 0.f)
		m_CurrentCoilyCooldown -= elapsedSec;
	if (m_pCoily.expired() && m_CurrentCoilyCooldown <= 0.f)
		SpawnCoily();

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
	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(m_pScoreObserver);
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
				m_pPlayers.first.lock()->Reset(m_pGrid.lock()->GetCoopStartNodes().first);
			if (!m_pPlayers.second.expired())
				m_pPlayers.second.lock()->Reset(m_pGrid.lock()->GetCoopStartNodes().second);
			break;
		case GameMode::Versus:
			if (!m_pPlayers.first.expired())
				m_pPlayers.first.lock()->Reset(m_pGrid.lock()->GetSoloStartNode());
			break;
	}
}

void GameManagerComponent::LoadSingle()
{
	SetEnemyCooldown(10, 7.5, 15);
	
	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetSoloStartNode()));
	
	auto lifeCounter = std::make_shared<engine::GameObject>();
	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter->AddComponent<engine::CounterComponent>(std::make_shared<engine::CounterComponent>(lifeCounter, font, obj->GetComponent<PlayerComponent>().lock()->GetLives(), "Lives: "));
	lifeCounter->SetPosition(m_pOwner.lock()->GetPosition().x - 100.f, m_pOwner.lock()->GetPosition().y - 50.f);
	m_pScene.lock()->Add(lifeCounter);

	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(std::make_shared<LifeObserver>(lifeCounter->GetComponent<engine::CounterComponent>(), m_pOwner));
	m_pPlayers.first = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj);

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_Y, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_B, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_A, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_X, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_a, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_z, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_s, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_q, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));
}

void GameManagerComponent::LoadCoop()
{
	SetEnemyCooldown(10, 7.5, 15);
	
	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetCoopStartNodes().first));

	auto lifeCounter = std::make_shared<engine::GameObject>();
	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter->AddComponent<engine::CounterComponent>(std::make_shared<engine::CounterComponent>(lifeCounter, font, obj->GetComponent<PlayerComponent>().lock()->GetLives(), "Lives: "));
	lifeCounter->SetPosition(m_pOwner.lock()->GetPosition().x - 100.f, m_pOwner.lock()->GetPosition().y - 50.f);
	m_pScene.lock()->Add(lifeCounter);

	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(std::make_shared<LifeObserver>(lifeCounter->GetComponent<engine::CounterComponent>(), m_pOwner));
	m_pPlayers.first = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj);

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_Y, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_B, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_A, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_X, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_a, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_z, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_s, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_q, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));
	
	obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert2.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetCoopStartNodes().second));

	lifeCounter = std::make_shared<engine::GameObject>();
	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter->AddComponent<engine::CounterComponent>(std::make_shared<engine::CounterComponent>(lifeCounter, font, obj->GetComponent<PlayerComponent>().lock()->GetLives(), "Lives: "));
	lifeCounter->SetPosition(m_pOwner.lock()->GetPosition().x + 150.f, m_pOwner.lock()->GetPosition().y - 50.f);
	m_pScene.lock()->Add(lifeCounter);
	
	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(std::make_shared<LifeObserver>(lifeCounter->GetComponent<engine::CounterComponent>(), m_pOwner));
	m_pPlayers.second = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj);

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_UP, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_RIGHT, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_DOWN, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_LEFT, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::BOTTOMLEFT));
	
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_o, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_p, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_m, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_l, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.second, engine::Direction::BOTTOMLEFT));
}

void GameManagerComponent::LoadVersus()
{
	SetEnemyCooldown(10, 7.5, 5);

	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(std::make_shared<engine::RenderComponent>(obj, "Qbert.png", engine::Float2{ 0.f, -32.f }));
	obj->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(obj, m_pGrid.lock()->GetSoloStartNode()));

	auto lifeCounter = std::make_shared<engine::GameObject>();
	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter->AddComponent<engine::CounterComponent>(std::make_shared<engine::CounterComponent>(lifeCounter, font, obj->GetComponent<PlayerComponent>().lock()->GetLives(), "Lives: "));
	lifeCounter->SetPosition(m_pOwner.lock()->GetPosition().x - 100.f, m_pOwner.lock()->GetPosition().y - 50.f);
	m_pScene.lock()->Add(lifeCounter);

	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(std::make_shared<LifeObserver>(lifeCounter->GetComponent<engine::CounterComponent>(), m_pOwner));
	m_pPlayers.first = obj->GetComponent<PlayerComponent>();
	m_pScene.lock()->Add(obj);

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_LTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_Y, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_B, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_A, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_X, engine::InputTriggerType::OnInputDown, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_a, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPLEFT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_z, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::TOPRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_s, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMRIGHT));
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_q, engine::InputTriggerType::OnInputHold, std::make_shared<PlayerMove>(m_pPlayers.first, engine::Direction::BOTTOMLEFT));

	auto command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::TOPLEFT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_UPLEFT, engine::InputTriggerType::OnInputHold, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::TOPRIGHT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_UPRIGHT, engine::InputTriggerType::OnInputHold, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::BOTTOMRIGHT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_DOWNRIGHT, engine::InputTriggerType::OnInputHold, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::BOTTOMLEFT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_RTHUMB_DOWNLEFT, engine::InputTriggerType::OnInputHold, command);

	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::TOPLEFT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_UP, engine::InputTriggerType::OnInputDown, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::TOPRIGHT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_RIGHT, engine::InputTriggerType::OnInputDown, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::BOTTOMRIGHT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_DOWN, engine::InputTriggerType::OnInputDown, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::BOTTOMLEFT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddControllerCommand(VK_PAD_DPAD_LEFT, engine::InputTriggerType::OnInputDown, command);

	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::TOPLEFT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_o, engine::InputTriggerType::OnInputHold, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::TOPRIGHT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_p, engine::InputTriggerType::OnInputHold, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::BOTTOMRIGHT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_m, engine::InputTriggerType::OnInputHold, command);
	command = std::make_shared<CoilyMove>(std::shared_ptr<CoilyComponent>(nullptr), engine::Direction::BOTTOMLEFT);
	m_pCoilyCommands.push_back(command);
	engine::InputManager::GetInstance().AddKeyBoardCommand(SDLK_l, engine::InputTriggerType::OnInputHold, command);
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

	auto obj = std::make_shared<engine::GameObject>();
	
	switch (m_GameMode)
	{
		case GameMode::Single:
			obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, true, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
			break;
		case GameMode::Coop:
			if (rand() % 2)
				obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, true, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
			else
				obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, true, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.second));
			break;
		case GameMode::Versus:
			obj->AddComponent<CoilyComponent>(std::make_shared<CoilyComponent>(obj, std::pair<std::string, std::string>{ "Egg.png", "Coily.png" }, false, m_pGrid.lock()->GetSoloStartNode(), m_pPlayers.first));
			UpdateCoilyCommands(obj->GetComponent<CoilyComponent>());
			break;
	}
	
	m_pCoily = obj->GetComponent<CoilyComponent>();
	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(m_pScoreObserver);
	m_pScene.lock()->Add(obj);
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

void GameManagerComponent::CheckCharacterOverlap()
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

void GameManagerComponent::UpdateCoilyCommands(std::weak_ptr<CoilyComponent> pTarger)
{
	for(auto command : m_pCoilyCommands)
		command.lock()->ChangeTarget(pTarger);
}