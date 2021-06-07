#include "Game.h"

#include <cstdlib>
#include <ctime>

#include "Audio.h"
#include "AudioLocator.h"
#include "DebugManager.h"
#include "GameObject.h"
#include "QbertMenuComponent.h"
#include "Scene.h"
#include "SceneManager.h"

void Game::LoadGame() const
{
	srand(static_cast<unsigned>(time(nullptr)));
	
	engine::DebugManager::GetInstance().SetChannelDeactivated(true);
	engine::AudioLocator::getAudioSystem()->SetVolume(1);
	
	auto pScene = engine::SceneManager::GetInstance().CreateScene("Qbert").lock();
	engine::SceneManager::GetInstance().SetActiveScene("Qbert");
	
	auto obj = std::make_shared<engine::GameObject>();
	obj->AddComponent<QbertMenuComponent>(std::make_shared<QbertMenuComponent>(obj, pScene));
	pScene->Add(obj);
}