#pragma once

#include <string>
#include <memory>
#include <SDL_ttf.h>

struct SDL_Color;
namespace dae
{
	class FontAsset;
	class Text final
	{
	public:
		
		Text(std::weak_ptr<FontAsset> pFont, const std::string& text, uint32_t size,const glm::fvec4& FGColor,const glm::fvec4& BGColor);
		Text(const std::string& assetPath, const std::string& fontName, const std::string& text, uint32_t size,const glm::fvec4& FGColor,const glm::fvec4& BGColor);
		
		//void SetText(const std::string& text) { m_Text = text; }
		//void SetSize(uint32_t value) { m_Size = value; }
		//void SetColor(const glm::fvec4& color) { m_Color = color; }
		//void SetFontAsset(std::weak_ptr<FontAsset> pFontAsset) { m_pFontAsset = pFontAsset; }
		RenderTexture GetRenderTexture();
		//std::shared_ptr<FontAsset> GetFontAsset() const { return m_pFontAsset.lock(); }
		//std::shared_ptr<TTF_Font> GetSDLFont() const;
		//uint32_t GetSize() const { return m_Size; }
		//const std::string& GetString() const { return m_Text; }
		//const glm::fvec4& GetColor() const { return m_Color; }

	private:
		
		std::weak_ptr<FontAsset> m_pFontAsset;
		glm::fvec4 m_FGColor;
		glm::fvec4 m_BGColor;
		std::string m_Text;
		uint32_t m_Size;
	};
}


