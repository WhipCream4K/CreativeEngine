#pragma once
#include "IFontEntity.h"

#include <sstream>

namespace dae
{
	struct RenderTransform;
	class SDLFontData final : public IFontEntity
	{
	public:

		SDLFontData(const std::string& assetPath, const std::string& assetName);

		//static constexpr void SetFontPtLimit(uint32_t value) { m_FontPtLimits = value; }

		std::shared_ptr<TTF_Font> GetSDLFont(uint32_t size);
		
	private:

		std::unordered_map<uint32_t, std::shared_ptr<TTF_Font>> m_pFonts;
		static constexpr uint32_t m_FontPtLimits{ 100 };
		
		void Render(const TextInfo& textInfo, const RenderTransform& transform,int renderFlags) override;
		void Init();
	};
}

