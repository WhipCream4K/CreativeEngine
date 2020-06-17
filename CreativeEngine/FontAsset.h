#pragma once
#include "BaseAsset.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <SDL_ttf.h>

namespace dae
{
	class FontAsset : public BaseAsset
	{
	public:
		
		FontAsset(const std::string& assetPath, const std::string& assetName);

		static std::shared_ptr<FontAsset> Create(const std::string& assetPath, const std::string& assetName);
		
		// TODO: Probably update this to support parsing font styles
		// SDL needs to load new font if you want to increase the size of the text
		std::shared_ptr<TTF_Font> GetSDLFont(uint32_t size);

		static void SetFontPtLimit(uint32_t value) { m_FontPtLimit = value; }
		
	private:

		// SDL font requires you to open new font if you have a different size of the font
		std::unordered_map<uint32_t, std::shared_ptr<TTF_Font>> m_pFonts;
		static uint32_t m_FontPtLimit;
	};
}

