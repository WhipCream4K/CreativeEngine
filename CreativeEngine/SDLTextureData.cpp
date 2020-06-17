#include "pch.h"
#include "SDLTextureData.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "RenderInfo.h"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

dae::SDLTextureData::SDLTextureData(const std::string& assetPath, const std::string& name)
	: ITextureEntity(assetPath, name)
	, m_pTextureData{}
{
	Init();
}

void dae::SDLTextureData::Render(const TextureInfo& textureInfo, const RenderTransform& transform, [[maybe_unused]] int renderFlags) const
{
	SDL_Rect renderBox{ EngineRectToSDLRect(transform.position,textureInfo.textureRect.z * transform.scale.x,textureInfo.textureRect.w * transform.scale.y) };
	SDL_Point pivotPoint{ Float2ToSDLPoint(textureInfo.pivot) };
	SDL_Rect subTextureBounding{ Float4ToSDLRect(textureInfo.textureRect) };

	int flipMode{ SDL_FLIP_NONE };
	if (textureInfo.isFlipX)
		flipMode |= SDL_FLIP_HORIZONTAL;
	if (textureInfo.isFlipY)
		flipMode |= SDL_FLIP_VERTICAL;
	
	// TODO: Now this here we would get the renderer service but i don't have another service
	SDL_RenderCopyEx(Renderer::GetSDLRenderer().get(), m_pTextureData.get(),
		&subTextureBounding,
		&renderBox,
		double(transform.rotation),
		&pivotPoint,
		SDL_RendererFlip(flipMode));
}

void dae::SDLTextureData::Init()
{
	if(!m_pTextureData)
	{
		m_pTextureData = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(Renderer::GetSDLRenderer().get(), GetPath().c_str()),SDLDeleter());
	}
}

glm::fvec2 dae::SDLTextureData::GetActualDimensionAfterLoad() const
{
	if(m_pTextureData)
	{
		int x{}, y{};
		SDL_QueryTexture(m_pTextureData.get(), nullptr, nullptr, &x, &y);
		return glm::fvec2{ float(x),float(y) };
	}
	
	return glm::fvec2{};
}
