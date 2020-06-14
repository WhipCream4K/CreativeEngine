#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Renderer;
namespace dae
{
	class SceneManager;
	class CreativeCore
	{
	public:
		CreativeCore();
		virtual ~CreativeCore() = default;

		void Run();
		virtual void GameInitialize() = 0;
		virtual void CleanUp();
		
	private:
		std::shared_ptr<SDL_Window> m_pWindow;
		std::future<void> m_FutureUpdate;
	};
}


