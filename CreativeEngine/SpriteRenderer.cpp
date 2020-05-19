#include "pch.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Sprite.h"

#include <SDL.h>

dae::SpriteRenderer::SpriteRenderer(const Sprite& sprite)
	: m_Sprite{ sprite }
{
}

void dae::SpriteRenderer::Render()
{
	SDL_Rect textureRect{};
	textureRect.w = int(m_Sprite.GetWidth());
	textureRect.h = int(m_Sprite.GetHeight());
	
	SDL_Texture* texture{ m_Sprite.GetSDLTexture() };
	const auto& renderer{ Renderer::GetSDLRenderer() };
	
	// Render that texture
	if (SDL_RenderCopy(renderer, texture, nullptr, &textureRect))
		throw std::runtime_error(std::string("Failed to render : " + m_Sprite.GetTexture2D()->GetPath()) + SDL_GetError());
	
}
