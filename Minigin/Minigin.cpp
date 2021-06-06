#include "MiniginPCH.h"
#include "Minigin.h"

#include <vld.h>
#include <SDL.h>
#include <chrono>

#include "AudioLocator.h"
#include "AudioLogger.h"
#include "AudioService.h"
#include "EngineTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"

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