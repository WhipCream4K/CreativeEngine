#include "pch.h"
#include "Renderer.h"

#include <stdexcept>
#include <SDL.h>

dae::Renderer::~Renderer()
{
	DestroyImpl();
}

void dae::Renderer::InitializeImpl(SDL_Window* pWindow)
{
	if (pWindow)
		m_pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_pRenderer)
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

void dae::Renderer::DestroyImpl()
{
	if (m_pRenderer)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}
