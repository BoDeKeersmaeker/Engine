#include "Game.h"
#include "MiniginPCH.h"

#pragma region Core
#include "GameObject.h"
#include "Scene.h"
#pragma endregion

#pragma region Components
#include "TextComponent.h"
#include "RenderComponent.h"
#include "PlayerComponent.h"
#include "SubjectComponent.h"
#include "FPSComponent.h"
#include "CounterComponent.h"
#include "GridComponent.h"
#include "GreenEnemyComponent.h"
#include "PurpleEnemyComponent.h"
#include "CoilyComponent.h"
#include "GameManagerComponent.h"
#pragma endregion 

#pragma region Commands
#include "Kill.h"
#include "ColorChange.h"
#include "CoilyDiscBait.h"
#include "CatchSlick.h"
#include "CatchSam.h"
#include "Move.h"
#pragma endregion 

#pragma region Observers
#include "PlayerDeathObserver.h"
#include "PlayerScoreObserver.h"
#pragma endregion

#pragma region Audio
#include "Audio.h"
#include "AudioLocator.h"
#pragma endregion

#pragma region Helpers
#include "DebugManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Structs.h"
#pragma endregion

using namespace std;

void Game::LoadGame() const
{
	engine::DebugManager::GetInstance().SetChannelDeactivated(false);

	engine::AudioLocator::getAudioSystem()->AddMusic(0, "./../Data/QbertDead.wav");
	engine::AudioLocator::getAudioSystem()->SetVolume(1);

	auto pScene = engine::SceneManager::GetInstance().CreateScene("Qbert").lock();
	engine::SceneManager::GetInstance().SetActiveScene("Qbert");

	auto obj = make_shared<engine::GameObject>();
	obj->SetPosition(275.f, 100.f);
	obj->AddComponent<GameManagerComponent>(make_shared<GameManagerComponent>(obj, pScene, GameMode::Single, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
	pScene->Add(obj);
}