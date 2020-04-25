#pragma once

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class SceneManager;
	class CreativeCore
	{
	public:
		void Run();
	private:
		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;
	};
}


