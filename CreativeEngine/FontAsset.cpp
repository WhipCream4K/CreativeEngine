#include "pch.h"
#include "FontAsset.h"
#include "ResourceManager.h"

uint32_t dae::FontAsset::m_FontPtLimit{ 100 };

dae::FontAsset::FontAsset(const std::string& assetPath, const std::string& assetName)
	: BaseAsset(assetPath, assetName)
	, m_pFonts{}
{
	for (uint32_t i = 1; i <= m_FontPtLimit; ++i)
	{
		const auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(assetPath.c_str(), i), SDLDeleter());
		if (!font)
			throw std::runtime_error(std::string("FontAsset::FontAsset() failed to initialize SDL_TTF ") + assetPath + SDL_GetError());
		m_pFonts.try_emplace(i, font);
	}
}

std::shared_ptr<dae::FontAsset> dae::FontAsset::Create(const std::string& assetPath, const std::string& assetName)
{
}

std::shared_ptr<TTF_Font> dae::FontAsset::GetSDLFont(uint32_t size)
{
	const auto it = m_pFonts.find(size);

	if (it != m_pFonts.end())
		return it->second;

	auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(GetPath().c_str(), int(size)), SDLDeleter());
	if (!font)
		throw std::runtime_error(std::string("FontAsset::GetSDLFont(uint32_t) failed to initialize SDL_TTF ") + GetPath() + SDL_GetError());
	m_pFonts.try_emplace(size, font);

	return font;
}
