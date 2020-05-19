#include "pch.h"
#include "CreativeCore.h"

#include <iostream>
#include <SDL.h>
#include <SDL_render.h>
#include <vld.h>

#include "SceneManager.h"
#include "EngineContext.h"
#include "Renderer.h"

void dae::CreativeCore::Run()
{
	try
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

		const auto windowSettings{ EngineContext::GetWindowSettings() };
		
		SDL_Window* pWindow{SDL_CreateWindow(
			windowSettings.title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			windowSettings.width,
			windowSettings.height,
			SDL_WINDOW_OPENGL)};
	

		Renderer::Initialize(pWindow);
		SDL_Renderer* pRenderer{ Renderer::GetSDLRenderer() };
		
		// game loop
		const bool& isApplicationExit{ EngineContext::IsApplicationExit() };
		const SDL_Color blue{ 173,216,230,255 };
		while(!isApplicationExit)
		{
			SDL_Event windowEvent;
			while (SDL_PollEvent(&windowEvent))
			{
				switch (windowEvent.type)
				{
				case SDL_QUIT:
					EngineContext::SetApplicationExit(true);
					break;
				}
			}
			
			auto &pSceneManager{ SceneManager::GetInstance() };
			
			pSceneManager.Update();

			SDL_SetRenderDrawColor(pRenderer, blue.r, blue.g, blue.b, blue.a);

			SDL_RenderClear(pRenderer);

			pSceneManager.Render();

			SDL_RenderPresent(pRenderer);
		}

		// TODO: I personally don't like this, maybe in the future I'll change the way to destroy SDL Objects
		Renderer::Destroy();
		SDL_DestroyWindow(pWindow);
		SDL_Quit();
	}
	catch (const std::runtime_error& result)
	{
		std::cerr << result.what() << std::endl;
	}
}
