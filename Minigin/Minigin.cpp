#include "MiniginPCH.h"
#include "Minigin.h"

#include <SDL.h>
#include <chrono>

#pragma region Core
#include "Renderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"
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

using namespace std;
using namespace std::chrono;

void engine::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

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
}

/**
 * Code constructing the scene world starts here
 */
void engine::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_shared<GameObject>();
	background->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(background, "background.jpg"));
	scene.Add(background);

	auto logo = std::make_shared<GameObject>();
	logo->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(logo, "logo.png"));
	logo->SetPosition(216, 180);
	scene.Add(logo);

	auto text = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	text->AddComponent<TextComponent>(std::make_shared<TextComponent>(text, "Programming 4 Assignment", font));
	text->SetPosition(80, 20);
	scene.Add(text);

	auto fpsCounter = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	fpsCounter->AddComponent<FPSComponent>(std::make_shared<FPSComponent>(fpsCounter, font));
	fpsCounter->SetPosition(10, 10);
	scene.Add(fpsCounter);

	auto lifeCounter1 = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter1->AddComponent<LivesComponent>(std::make_shared<LivesComponent>(lifeCounter1, font));
	lifeCounter1->SetPosition(540, 10);
	scene.Add(lifeCounter1);

	auto lifeCounter2 = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	lifeCounter2->AddComponent<LivesComponent>(std::make_shared<LivesComponent>(lifeCounter2, font));
	lifeCounter2->SetPosition(340, 10);
	scene.Add(lifeCounter2);

	auto scoreCounter1 = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	scoreCounter1->AddComponent<ScoreComponent>(std::make_shared<ScoreComponent>(scoreCounter1, font));
	scoreCounter1->SetPosition(540, 380);
	scene.Add(scoreCounter1);

	auto scoreCounter2 = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	scoreCounter2->AddComponent<ScoreComponent>(std::make_shared<ScoreComponent>(scoreCounter2, font));
	scoreCounter2->SetPosition(340, 380);
	scene.Add(scoreCounter2);
	
	auto Player1 = std::make_shared<GameObject>();
	Player1->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(Player1));
	Player1->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(Player1, "tempQbert2.jpg"));
	Player1->GetComponent<SubjectComponent>().lock()->AddObserver(std::make_shared<PlayerDeathObserver>(lifeCounter1->GetComponent<LivesComponent>()));
	Player1->GetComponent<SubjectComponent>().lock()->AddObserver(std::make_shared<PlayerScoreObserver>(scoreCounter1->GetComponent<ScoreComponent>()));
	scene.Add(Player1);

	auto Player2 = std::make_shared<GameObject>();
	Player2->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(Player2));
	Player2->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(Player2, "tempQbert2.jpg"));
	Player2->GetComponent<SubjectComponent>().lock()->AddObserver(std::make_shared<PlayerDeathObserver>(lifeCounter2->GetComponent<LivesComponent>()));
	Player2->GetComponent<SubjectComponent>().lock()->AddObserver(std::make_shared<PlayerScoreObserver>(scoreCounter2->GetComponent<ScoreComponent>()));
	scene.Add(Player2);
	
	InputManager::GetInstance().AddCommand({ VK_PAD_RSHOULDER, InputTriggerType::OnInputDown }, std::make_shared<Kill>(Player1->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_A, InputTriggerType::OnInputDown }, std::make_shared<ColorChange>(Player1->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_B, InputTriggerType::OnInputDown }, std::make_shared<CoilyDiscBait>(Player1->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_X, InputTriggerType::OnInputDown }, std::make_shared<CatchSlick>(Player1->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_Y, InputTriggerType::OnInputDown }, std::make_shared<CatchSam>(Player1->GetComponent<PlayerComponent>()));
	
	InputManager::GetInstance().AddCommand({ VK_PAD_LSHOULDER, InputTriggerType::OnInputDown }, std::make_shared<Kill>(Player2->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_DOWN, InputTriggerType::OnInputDown }, std::make_shared<ColorChange>(Player2->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_RIGHT, InputTriggerType::OnInputDown }, std::make_shared<CoilyDiscBait>(Player2->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_LEFT, InputTriggerType::OnInputDown }, std::make_shared<CatchSlick>(Player2->GetComponent<PlayerComponent>()));
	InputManager::GetInstance().AddCommand({ VK_PAD_DPAD_UP, InputTriggerType::OnInputDown }, std::make_shared<CatchSam>(Player2->GetComponent<PlayerComponent>()));
}

void engine::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
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
			Time::GetInstance().UpdateElapsedSec(duration_cast<duration<float>>(currentTime - lastTime).count());

			doContinue = input.ProcessInput();
			sceneManager.Update();
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}