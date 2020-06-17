#pragma once

#include "IDrawable.h"

struct SDL_Color;
namespace dae
{
	class IFontEntity;
	class Text final : public IDrawable
	{
	public:

		Text();
		Text(std::weak_ptr<IFontEntity>&& fontAsset);
		//static Text Create(const std::string& fontPath, const std::string& fontName);
		static auto Create(const std::string& fontPath, const std::string& fontName)->std::shared_ptr<Text>;
		static auto Create(std::weak_ptr<IFontEntity>&& fontEntity, const std::string& text,uint32_t size)->std::shared_ptr<Text>;
		
		// Getter
		const std::string& GetText() const { return m_Text; }
		const glm::fvec4& GetForegroundColor() const { return m_FGColor; }
		const glm::fvec4& GetBackgroundColor() const { return m_BGColor; }
		uint32_t GetTextSize() const { return m_Size; }

		// Setter
		void SetFontAsset(std::weak_ptr<IFontEntity>&& font) { m_pFontAsset = std::move(font); }
		void SetForegroundColor(const glm::fvec4& color) { m_FGColor = color; }
		void SetBackgroundColor(const glm::fvec4& color) { m_BGColor = color; }
		void SetTextSize(uint32_t value) { m_Size = value; }
		void SetText(const std::string& text) { m_Text = text; }
		
	private:
		
		std::weak_ptr<IFontEntity> m_pFontAsset;
		glm::fvec4 m_FGColor;
		glm::fvec4 m_BGColor;
		std::string m_Text;
		uint32_t m_Size;
		
		void Render(const RenderTransform& transform) const override;
	};
}


