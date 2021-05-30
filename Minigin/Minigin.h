#pragma once
#include <vector>

struct SDL_Window;
namespace engine
{
	class GameObject;

	class Minigin
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();

	private:
		void LoadInput(const std::vector<std::shared_ptr<GameObject>>& pPlayers) const;
		
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
	};
}