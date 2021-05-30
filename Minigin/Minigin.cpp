#include "MiniginPCH.h"
#include "Minigin.h"

#include <vld.h>
#include <SDL.h>
#include <chrono>

#pragma region Core
#include "Renderer.h"
#include "GameObject.h"
#include "Scene.h"
#pragma endregion

#pragma region Managers
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#pragma endregion 

#pragma region Components
#include "TextComponent.h"
#include "RenderComponent.h"
#include "PlayerComponent.h"
#include "SubjectComponent.h"
#include "FPSComponent.h"
#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "CounterComponent.h"
#include "GridComponent.h"
#pragma endregion 

#pragma region Commands
#include "Kill.h"
#include "ColorChange.h"
#include "CoilyDiscBait.h"
#include "CatchSlick.h"
#include "CatchSam.h"
#pragma endregion 

#pragma region Observers
#include "PlayerDeathObserver.h"
#include "PlayerScoreObserver.h"
#pragma endregion

#pragma region Audio
#include "AudioLocator.h"
#include "Audio.h"
#include "AudioService.h"
#include "AudioLogger.h"
#include "EngineTime.h"
#pragma endregion

using namespace std;
using namespace std::chrono;

void engine::Minigin::Initialize()
{
	_putenv("SDL_AUDIODRIVER=DirectSound");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	if(SDL_Init(SDL_INIT_AUDIO) !=0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cerr << "Core::Initialize( ), error when calling Mix_OpenAudio: " << Mix_GetError() << std::endl;
		return;
	}

	const int mixerFlags{ MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG };
	if ((Mix_Init(mixerFlags) & mixerFlags) != mixerFlags)
		std::cerr << "SDL Mixer failed to initialize! Cause of the error: " << Mix_GetError();

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	AudioLocator::registerAudio(new AudioLogger{ new AudioService{} });
}

void engine::Minigin::LoadGame() const
{
	engine::AudioLocator::getAudioSystem()->AddMusic(0, "./../Data/QbertDead.wav");

	auto& scene = engine::SceneManager::GetInstance().CreateScene("Demo");

	auto obj = make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(obj, "background.jpg"));
	scene.Add(obj);

	obj = make_shared<engine::GameObject>();
	obj->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(obj, "logo.png"));
	obj->SetPosition(216.f, 180.f);
	scene.Add(obj);

	obj = make_shared<engine::GameObject>();
	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	obj->AddComponent<engine::TextComponent>(make_shared<engine::TextComponent>(obj, "Programming 4 Assignment", font));
	obj->SetPosition(80.f, 20.f);
	scene.Add(obj);

	obj = make_shared<engine::GameObject>();
	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	obj->AddComponent<engine::FPSComponent>(make_shared<engine::FPSComponent>(obj, font));
	obj->SetPosition(10.f, 10.f);
	scene.Add(obj);

	auto lifeCounter1 = make_shared<engine::GameObject>();
	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter1->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(lifeCounter1, font, 5, "Player 1: ", " Lives left", "Game over"));
	lifeCounter1->SetPosition(440.f, 60.f);
	scene.Add(lifeCounter1);

	auto lifeCounter2 = make_shared<engine::GameObject>();
	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter2->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(lifeCounter2, font, 5, "Player 2: ", " Lives left", "Game over"));
	lifeCounter2->SetPosition(240.f, 60.f);
	scene.Add(lifeCounter2);

	auto scoreCounter1 = make_shared<engine::GameObject>();
	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	scoreCounter1->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(scoreCounter1, font, 0, "Player 1 score: "));
	scoreCounter1->SetPosition(440.f, 430.f);
	scene.Add(scoreCounter1);

	auto scoreCounter2 = make_shared<engine::GameObject>();
	font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	scoreCounter2->AddComponent<engine::CounterComponent>(make_shared<engine::CounterComponent>(scoreCounter2, font, 0, "Player 2 score: "));
	scoreCounter2->SetPosition(240.f, 430.f);
	scene.Add(scoreCounter2);

	auto Player1 = make_shared<engine::GameObject>();
	Player1->AddComponent<engine::PlayerComponent>(make_shared<engine::PlayerComponent>(Player1));
	Player1->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(Player1, "tempQbert2.jpg"));
	Player1->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerDeathObserver>(lifeCounter1->GetComponent<engine::CounterComponent>()));
	Player1->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerScoreObserver>(scoreCounter1->GetComponent<engine::CounterComponent>()));
	scene.Add(Player1);

	auto Player2 = make_shared<engine::GameObject>();
	Player2->AddComponent<engine::PlayerComponent>(make_shared<engine::PlayerComponent>(Player2));
	Player2->AddComponent<engine::RenderComponent>(make_shared<engine::RenderComponent>(Player2, "tempQbert2.jpg"));
	Player2->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerDeathObserver>(lifeCounter2->GetComponent<engine::CounterComponent>()));
	Player2->GetComponent<engine::SubjectComponent>().lock()->AddObserver(make_shared<engine::PlayerScoreObserver>(scoreCounter2->GetComponent<engine::CounterComponent>()));
	scene.Add(Player2);
	
	LoadInput({ Player1, Player2 });

	obj = make_shared<engine::GameObject>();
	obj->SetPosition(100.f, 200.f);
	auto comp = make_shared<engine::GridComponent>(obj, std::vector<std::string>{ "Block_Pink.png", "Block_Blue.png" });
	obj->AddComponent<engine::GridComponent>(comp);
	DebugManager::GetInstance().SetChannelActivated(false, NODE_DEBUG);
	DebugManager::GetInstance().SetChannelActivated(true, GRID_DEBUG);
	comp->GenerateGrid(&scene, 7, 24.f);
	scene.Add(obj);


}

void engine::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();

	delete AudioLocator::getAudioSystem();
}

void engine::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();

		bool doContinue = true;
		auto lastTime = std::chrono::high_resolution_clock::now();
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			EngineTime::GetInstance().UpdateElapsedSec(duration_cast<duration<float>>(currentTime - lastTime).count());

			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}

void engine::Minigin::LoadInput(const vector<shared_ptr<GameObject>>& pPlayers) const
{
	if (pPlayers.size() < 1)
		return;
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_RSHOULDER, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::Kill>(pPlayers[0]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_A, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::ColorChange>(pPlayers[0]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_B, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::CoilyDiscBait>(pPlayers[0]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_X, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::CatchSlick>(pPlayers[0]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_Y, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::CatchSam>(pPlayers[0]->GetComponent<engine::PlayerComponent>()));

	if (pPlayers.size() < 2)
		return;
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_LSHOULDER, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::Kill>(pPlayers[1]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_DOWN, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::ColorChange>(pPlayers[1]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_RIGHT, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::CoilyDiscBait>(pPlayers[1]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_LEFT, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::CatchSlick>(pPlayers[1]->GetComponent<engine::PlayerComponent>()));
	engine::InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_UP, engine::InputTriggerType::OnInputDown }, std::make_shared<engine::CatchSam>(pPlayers[1]->GetComponent<engine::PlayerComponent>()));
}