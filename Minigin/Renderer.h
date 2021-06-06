#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;

namespace engine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
		int GetOpenGLDriverIndex();
	
	private:
		void RenderImguiDemo();
		void RenderGameModeWindow();
		
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_pWindow = nullptr;
		bool m_ShowDemo = false;
		bool m_ShowHowToPlay = true;
		bool m_ShowStartMenu = true;
	};
}

