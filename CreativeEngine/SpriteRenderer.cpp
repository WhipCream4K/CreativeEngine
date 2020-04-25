#include "pch.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include <SDL.h>

void dae::SpriteRenderer::Render()
{
	SDL_Rect textureRect{};
	SDL_Texture* mySDLTexture{ m_Texture->GetSDLTexture() };
	SDL_QueryTexture(mySDLTexture, nullptr, nullptr, &textureRect.w, &textureRect.h);
	SDL_RenderCopy(Renderer::GetInstance().GetSDLRenderer(), m_Texture->GetSDLTexture(), nullptr, &textureRect);
}

void dae::SpriteRenderer::SetSpriteTexture(const std::string& path)
{
	m_Texture = ResourceManager::Load<Texture2D>(path);
}

void dae::SpriteRenderer::SetSpriteTexture(std::shared_ptr<Texture2D> pTexture)
{
	if (pTexture)
		m_Texture = pTexture;
}
