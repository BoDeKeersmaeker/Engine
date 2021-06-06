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
#include "Move.h"
#pragma endregion 

#pragma region Observers

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
	engine::DebugManager::GetInstance().SetChannelDeactivated(true);

	engine::AudioLocator::getAudioSystem()->AddMusic(0, "./../Data/QbertDead.wav");
	engine::AudioLocator::getAudioSystem()->SetVolume(1);

	auto pScene = engine::SceneManager::GetInstance().CreateScene("Qbert").lock();
	engine::SceneManager::GetInstance().SetActiveScene("Qbert");

	auto obj = make_shared<engine::GameObject>();
	obj->SetPosition(275.f, 100.f);
	obj->AddComponent<GameManagerComponent>(make_shared<GameManagerComponent>(obj, pScene, GameMode::Single, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
	pScene->Add(obj);
}

//void Game::LoadDemo() const
//{
//	engine::AudioLocator::getAudioSystem()->AddMusic(0, "./../Data/QbertDead.wav");
//
//	auto& scene = *engine::SceneManager::GetInstance().CreateScene("Demo").lock();
//
//	auto obj = make_shared<engine::GameObject>();
//	obj->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(obj, "background.jpg"));
//	scene.Add(obj);
//
//	obj = make_shared<engine::GameObject>();
//	obj->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(obj, "logo.png"));
//	obj->SetPosition(216.f, 180.f);
//	scene.Add(obj);
//
//	obj = make_shared<engine::GameObject>();
//	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
//	obj->AddComponent<engine::TextComponent>(make_shared<engine::TextComponent>(obj, "Programming 4 Assignment", font));
//	obj->SetPosition(80.f, 20.f);
//	scene.Add(obj);
//
//	obj = make_shared<engine::GameObject>();
//	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
//	obj->AddComponent<engine::FPSComponent>(make_shared<engine::FPSComponent>(obj, font));
//	obj->SetPosition(10.f, 10.f);
//	scene.Add(obj);
//
//	auto lifeCounter1 = make_shared<engine::GameObject>();
//	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
//	lifeCounter1->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(lifeCounter1, font, 5, "Player 1: ", " Lives left", "Game over"));
//	lifeCounter1->SetPosition(440.f, 60.f);
//	scene.Add(lifeCounter1);
//
//	auto lifeCounter2 = make_shared<engine::GameObject>();
//	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
//	lifeCounter2->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(lifeCounter2, font, 5, "Player 2: ", " Lives left", "Game over"));
//	lifeCounter2->SetPosition(240.f, 60.f);
//	scene.Add(lifeCounter2);
//
//	auto scoreCounter1 = make_shared<engine::GameObject>();
//	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
//	scoreCounter1->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(scoreCounter1, font, 0, "Player 1 score: "));
//	scoreCounter1->SetPosition(440.f, 430.f);
//	scene.Add(scoreCounter1);
//
//	auto scoreCounter2 = make_shared<engine::GameObject>();
//	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
//	scoreCounter2->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(scoreCounter2, font, 0, "Player 2 score: "));
//	scoreCounter2->SetPosition(240.f, 430.f);
//	scene.Add(scoreCounter2);
//
//	obj = make_shared<engine::GameObject>();
//	obj->AddComponent<PlayerComponent>(make_shared<PlayerComponent>(obj));
//	obj->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(obj, "tempQbert2.jpg"));
//	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerDeathObserver>(lifeCounter1->GetComponent<engine::CounterComponent>()));
//	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerScoreObserver>(scoreCounter1->GetComponent<engine::CounterComponent>()));
//	scene.Add(obj);
//
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_RSHOULDER, engine::InputTriggerType::OnInputDown, std::make_shared<engine::Kill>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_A, engine::InputTriggerType::OnInputDown, std::make_shared<engine::ColorChange>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_B, engine::InputTriggerType::OnInputDown, std::make_shared<engine::CoilyDiscBait>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_X, engine::InputTriggerType::OnInputDown, std::make_shared<engine::CatchSlick>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_Y, engine::InputTriggerType::OnInputDown, std::make_shared<engine::CatchSam>(obj->GetComponent<PlayerComponent>()));
//	obj = make_shared<engine::GameObject>();
//	obj->AddComponent<PlayerComponent>(make_shared<PlayerComponent>(obj));
//	obj->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(obj, "tempQbert2.jpg"));
//	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerDeathObserver>(lifeCounter2->GetComponent<engine::CounterComponent>()));
//	obj->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerScoreObserver>(scoreCounter2->GetComponent<engine::CounterComponent>()));
//	scene.Add(obj);
//
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_LSHOULDER, engine::InputTriggerType::OnInputDown, std::make_shared<engine::Kill>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_DPAD_DOWN, engine::InputTriggerType::OnInputDown, std::make_shared<engine::ColorChange>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_DPAD_RIGHT, engine::InputTriggerType::OnInputDown, std::make_shared<engine::CoilyDiscBait>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_DPAD_LEFT, engine::InputTriggerType::OnInputDown, std::make_shared<engine::CatchSlick>(obj->GetComponent<PlayerComponent>()));
//	engine::InputManager::GetInstance().AddCommand(VK_PAD_DPAD_UP, engine::InputTriggerType::OnInputDown, std::make_shared<engine::CatchSam>(obj->GetComponent<PlayerComponent>()));
//
//	obj = make_shared<engine::GameObject>();
//	obj->SetPosition(100.f, 200.f);
//	auto comp = make_shared<GridComponent>(obj, &scene, "./../Data/Level1.txt");
//	obj->AddComponent<GridComponent>(comp);
//	scene.Add(obj);
//}