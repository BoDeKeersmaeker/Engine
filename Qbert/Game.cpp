#include "Game.h"

#include <cstdlib>
#include <ctime>

#include "DebugManager.h"
#include "GameManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

void Game::LoadGame() const
{
	srand(static_cast<unsigned>(time(nullptr)));
	
	engine::DebugManager::GetInstance().SetChannelDeactivated(true);
	engine::DebugManager::GetInstance().SetChannelDeactivated(false, GAMEMANAGER_DEBUG);

	auto pScene = engine::SceneManager::GetInstance().CreateScene("Qbert").lock();
	engine::SceneManager::GetInstance().SetActiveScene("Qbert");
	
	auto obj = std::make_shared<engine::GameObject>();
	obj->SetPosition(275.f, 100.f);
	obj->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(obj, pScene, GameMode::Single, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
	pScene->Add(obj);
}