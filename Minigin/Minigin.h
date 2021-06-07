#pragma once

struct SDL_Window;
namespace engine
{
	class GameObject;

	class Minigin
	{
	public:
		Minigin() = default;
		virtual ~Minigin() = default;
		
		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
		
		void Initialize();
		void Cleanup();
		void Run();

	protected:
		virtual void LoadGame() const = 0;
	
	private:	
		SDL_Window* m_Window{};
	};
}