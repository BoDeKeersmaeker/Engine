#include "Game.h"

#include <cstdlib>
#include <ctime>

#include "Audio.h"
#include "AudioLocator.h"
#include "DebugManager.h"
#include "GameManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
//#include "MiniginPCH.h"

using namespace std;

void Game::LoadGame() const
{
	srand(static_cast<unsigned>(time(nullptr)));
	
	engine::DebugManager::GetInstance().SetChannelDeactivated(true);

	engine::AudioLocator::getAudioSystem()->AddMusic(0, "./../Data/QbertDead.wav");
	engine::AudioLocator::getAudioSystem()->SetVolume(1);

	auto pScene = engine::SceneManager::GetInstance().CreateScene("Qbert").lock();
	engine::SceneManager::GetInstance().SetActiveScene("Qbert");

	auto obj = make_shared<engine::GameObject>();
	obj->SetPosition(275.f, 100.f);
	obj->AddComponent<GameManagerComponent>(make_shared<GameManagerComponent>(obj, pScene, GameMode::Versus, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
	pScene->Add(obj);
}