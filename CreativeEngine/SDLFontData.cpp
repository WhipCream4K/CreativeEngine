#include "pch.h"
#include "SDLFontData.h"
#include "Renderer.h"
#include "SDLTextureData.h"
#include <SDL.h>

//uint32_t dae::SDLFontData::m_FontPtLimits{ 100 };

dae::SDLFontData::SDLFontData(const std::string& assetPath, const std::string& assetName)
	: IFontEntity(assetPath, assetName)
	, m_pFonts{}
{
	Init();
}

std::shared_ptr<TTF_Font> dae::SDLFontData::GetSDLFont(uint32_t size)
{
	const std::string_view path{ GetPath() };

	const auto it = m_pFonts.find(size);
	if (it != m_pFonts.end())
		return it->second;

	auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(path.data(), int(size)), SDLDeleter());
	if (!font)
		throw std::runtime_error(std::string("SDLFontData::GetSDLFont(uint32_t) failed to initialize SDL_TTF ") + path.data() + SDL_GetError());
	m_pFonts.try_emplace(size, font);

	return font;
}

void dae::SDLFontData::Render(const TextInfo& textInfo, const RenderTransform& transform, [[maybe_unused]] int renderFlags)
{
	const auto renderer{ Renderer::GetSDLRenderer().get() };

#pragma region Decieded not to include this
	// This Just prevents texture creation every frame
	// |
	// |
	// V

	//if (!m_pCurrentTexture || m_CurrentTextSize != textInfo.ptSize || m_CurrentFGColor != textInfo.fgColor)
	//{
	//	m_CurrentFGColor = textInfo.fgColor;
	//	const SDL_Color foregroundColor{ Float4ToSDLColor(textInfo.fgColor) };
	//	SDL_Surface* surface = TTF_RenderText_Blended(GetSDLFont(textInfo.ptSize).get(), textInfo.text.c_str(), foregroundColor);

	//	if(foregroundColor.a < 1)
	//	{
	//		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	//		SDL_SetSurfaceAlphaMod(surface, foregroundColor.a);
	//	}
	//	
	//	m_pCurrentTexture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface));
	//	int x{}, y{};
	//	SDL_QueryTexture(m_pCurrentTexture.get(), nullptr, nullptr, &x, &y);
	//	m_CurrentFontTextureSize = { float(x),float(y) };
	//	m_CurrentTextSize = textInfo.ptSize;
	//	SDL_FreeSurface(surface);
	//}
	//
#pragma endregion 

	const SDL_Color fgColor{ Float4ToSDLColor(textInfo.fgColor) };
	SDL_Surface* surface = TTF_RenderText_Blended(GetSDLFont(textInfo.ptSize).get(), textInfo.text.c_str(), fgColor);

	if (textInfo.fgColor.a < 1)
	{
		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
		SDL_SetSurfaceAlphaMod(surface, fgColor.a);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	const SDL_Rect renderBox{ EngineRectToSDLRect(transform.position,float(surface->w) * transform.scale.x,float(surface->h) * transform.scale.y) };
	const SDL_Point pivotPoint{ surface->w / 2,surface->h / 2 };

	// TODO: Implement text flip
	SDL_RenderCopyEx(renderer, texture, nullptr, &renderBox, transform.rotation, &pivotPoint, SDL_FLIP_NONE);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void dae::SDLFontData::Init()
{
	const std::string_view test{ GetPath() };

	for (uint32_t i = 1; i <= m_FontPtLimits; ++i)
	{
		const auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(test.data(), i), SDLDeleter());
		if (!font)
			throw std::runtime_error(std::string("SDLFontData::Init() failed to initialize SDL_TTF ") + test.data() + SDL_GetError());
		m_pFonts.try_emplace(i, font);
	}
}
