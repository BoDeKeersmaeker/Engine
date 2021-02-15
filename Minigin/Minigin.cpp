#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "Time.h"
#include "FPSComponent.h"
#include "vld.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
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
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<TextureComponent>());
	std::static_pointer_cast<TextureComponent>(go->GetComponent(ComponentType::Texture))->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<TextureComponent>());
	std::static_pointer_cast<TextureComponent>(go->GetComponent(ComponentType::Texture))->SetTexture("logo.png");
	go->SetPosition(216, 180);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go->AddComponent(std::make_shared<TextComponent>("Programming 4 Assignment", font));
	go->SetPosition(80, 20);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	go->AddComponent(std::make_shared<FPSComponent>(font));
	go->SetPosition(10, 10);
	scene.Add(go);
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
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
			//auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
			//this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}