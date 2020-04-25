#include "pch.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <SDL_image.h>


dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

std::shared_ptr<dae::Texture2D> dae::Texture2D::Load(const std::string& path)
{
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
	SDL_Texture* sdl_texture{ IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(),path.c_str()) };
	if(!sdl_texture)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	texture->m_pTexture = sdl_texture;
	return texture;
}
