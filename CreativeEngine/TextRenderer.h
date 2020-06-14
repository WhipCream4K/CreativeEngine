#pragma once
#include "BaseComponent.h"
#include "RenderTexture.h"

namespace dae
{
	class FontAsset;
	class TextRenderer : public BaseComponent
	{
	public:

		TextRenderer();
		
		void SetFontAsset(std::weak_ptr<FontAsset> pFontAsset);
		void SetFontAsset(const std::string& fontName);
		void SetText(const std::string& text);
		void SetSize(uint32_t value);
		void SetForegroundColor(const glm::fvec4& color);
		void SetBackgroundColor(const glm::fvec4& color) { m_BGColor = color; }
		//void SetPivotPoint(const glm::fvec2& pivot) { m_Pivot = pivot; }

		const std::string& GetText() const { return m_Text; }
		
	protected:
		
		void Render() const override;
		void LateUpdate() override;
		
	private:
		
		std::weak_ptr<FontAsset> m_pFont;
		RenderTexture m_RenderTexture;
		std::string m_Text;
		glm::fvec4 m_FGColor;
		glm::fvec4 m_BGColor;
		uint32_t m_Size;
		bool m_NeedsToCreateNewTexture;

		void CreateTexture();
	};
}

