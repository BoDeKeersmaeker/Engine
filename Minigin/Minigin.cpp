#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "TextComponent.h"
#include "Time.h"
#include "FPSComponent.h"
#include "Kill.h"
#include "RenderComponent.h"
#include "PlayerComponent.h"
#include "SubjectComponent.h"
#include "PlayerDeathObserver.h"
#include "LivesComponent.h"

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
	background->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(background));
	std::weak_ptr<RenderComponent> comp = background->GetComponent<RenderComponent>();
	if(comp.lock() != nullptr)
		comp.lock()->SetTexture("background.jpg");
	scene.Add(background);

	auto logo = std::make_shared<GameObject>();
	logo->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(logo));
	comp = logo->GetComponent<RenderComponent>();
	if (comp.lock() != nullptr)
		comp.lock()->SetTexture("logo.png");
	logo->SetPosition(216, 180);
	scene.Add(logo);

	auto tekst = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	tekst->AddComponent<TextComponent>(std::make_shared<TextComponent>(tekst, "Programming 4 Assignment", font));
	tekst->SetPosition(80, 20);
	scene.Add(tekst);

	auto fpsCounter = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	fpsCounter->AddComponent<FPSComponent>(std::make_shared<FPSComponent>(fpsCounter, font));
	fpsCounter->SetPosition(10, 10);
	scene.Add(fpsCounter);


	auto livesCounter = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	livesCounter->AddComponent<LivesComponent>(std::make_shared<LivesComponent>(livesCounter, font));
	livesCounter->SetPosition(100, 10);
	scene.Add(livesCounter);
	
	auto qBert = std::make_shared<GameObject>();
	qBert->AddComponent<PlayerComponent>(std::make_shared<PlayerComponent>(qBert, 0, 5));
	qBert->AddComponent<RenderComponent>(std::make_shared<RenderComponent>(qBert));
	qBert->GetComponent<SubjectComponent>().lock()->AddObserver(std::make_shared<PlayerDeathObserver>(livesCounter));
	comp = qBert->GetComponent<RenderComponent>();
	if (comp.lock() != nullptr)
		comp.lock()->SetTexture("tempQbert.jpg");
	qBert->SetPosition(100, 100);
	scene.Add(qBert);
	
	InputManager::GetInstance().AddCommand({ VK_PAD_A, InputTriggerType::OnInputDown }, std::make_shared<Kill>(qBert));
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
			//lateupdate?
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}