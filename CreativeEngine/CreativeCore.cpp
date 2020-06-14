#include "pch.h"
#include "CreativeCore.h"

#include <iostream>
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fmod.hpp>

#include "SceneManager.h"
#include "ResourceManager.h"
#include "EngineContext.h"
#include "Renderer.h"
#include "ServiceLocator.h"
#include "FMODAudioService.h"

dae::CreativeCore::CreativeCore()
	: m_pWindow{}
	, m_FutureUpdate{}
{
}

void dae::CreativeCore::Run()
{
	try
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			throw std::runtime_error(std::string("CreativeCore::Run() SDL_Init Error: ") + SDL_GetError());

		if (TTF_Init() != 0)
			throw std::runtime_error(std::string("CreativeCore::Run() SDL_TTF::Init() Error: ") + SDL_GetError());

		const auto windowSettings{ EngineContext::GetWindowSettings() };

		m_pWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			windowSettings.title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			windowSettings.width,
			windowSettings.height,
			SDL_WINDOW_OPENGL), SDLDeleter());

		if (!m_pWindow)
			throw std::runtime_error(std::string("CreativeCore::Run() SDL::CreateWindow() Error: ") + SDL_GetError());

		// Core Initialize
		Renderer::Initialize(m_pWindow);
		ServiceLocator::RegisterProvider(FMODAudioService::Create(EngineContext::GetMaxAudioChannels()));
		
		const auto t1 = std::chrono::high_resolution_clock::now();
		GameInitialize();

		const auto t2 = std::chrono::high_resolution_clock::now();

		const auto duration = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
		std::cout << duration << std::endl;

		const auto& pRenderer{ Renderer::GetSDLRenderer().get() };

		// game loop
		const bool& isApplicationExit{ EngineContext::IsApplicationExit() };
		const SDL_Color applicationColor{ Float4ToSDLColor(EngineContext::GetApplicationBGColor()) };
		while (!isApplicationExit)

		{
			SDL_Event windowEvent;
			while (SDL_PollEvent(&windowEvent))
			{
				switch (windowEvent.type)
				{
				case SDL_QUIT:
					EngineContext::SetApplicationExit(true);
					break;
				default:
					break;
				}
			}

			auto pSceneManager{ SceneManager::GetInstance() };

			pSceneManager->Update();

			SDL_SetRenderDrawColor(pRenderer, applicationColor.r, applicationColor.g, applicationColor.b, applicationColor.a);

			SDL_RenderClear(pRenderer);

			pSceneManager->Render();

			SDL_RenderPresent(pRenderer);
		}

		CleanUp();
	}
	catch (const std::runtime_error& result)
	{
		std::cerr << result.what() << std::endl;
	}
}

void dae::CreativeCore::CleanUp()
{
	ResourceManager::GetInstance()->Destroy();
	SceneManager::GetInstance()->Destroy();
	Renderer::GetInstance()->Destroy();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
