#include "pch.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <SDL_image.h>


dae::Texture2D::Texture2D(const std::string& assetPath)
	: BaseAsset(assetPath)
	, m_pTexture{}
{
	SDL_Texture* sdlTexture{ IMG_LoadTexture(Renderer::GetSDLRenderer(),assetPath.c_str()) };
	if (!sdlTexture)
		throw std::runtime_error(std::string("Failed to load texture: " + assetPath) + SDL_GetError());
	m_pTexture = sdlTexture;
}

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}
