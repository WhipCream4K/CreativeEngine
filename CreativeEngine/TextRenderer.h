#pragma once
#include "BaseComponent.h"
#include "Text.h"

namespace dae
{
	class IFontEntity;
	class TextRenderer : public BaseComponent
	{
	public:

		// Getter
		//std::shared_ptr<Text> GetText() const { return m_pTextObj; }
		auto GetText() const noexcept->std::shared_ptr<Text> { return m_pTextObj.lock(); }
		//Text& GetText() { return m_TextObj; }
		// Setter
		auto SetText(const std::string& text) noexcept -> void;
		auto SetText(std::weak_ptr<Text>&& text) noexcept -> void;
		auto SetFontAsset(std::weak_ptr<IFontEntity>&& fontAsset) noexcept -> void;
		auto SetTextSize(uint32_t value) noexcept -> void;
		auto SetForegroundColor(const glm::fvec4& color) noexcept -> void;
		auto SetBackgroundColor(const glm::fvec4& color) noexcept -> void;

		//void SetText(const std::shared_ptr<Text>& text) { m_pTextObj = text; }
		//void SetFontAsset(std::weak_ptr<FontAsset> pFontAsset);
		//void SetFontAsset(const std::string& fontName);
		//void SetText(const std::string& text);
		//void SetSize(uint32_t value);
		//void SetForegroundColor(const glm::fvec4& color);
		//void SetBackgroundColor(const glm::fvec4& color) { m_BGColor = color; }
		//void SetPivotPoint(const glm::fvec2& pivot) { m_Pivot = pivot; }

		//const std::string& GetText() const { return m_Text; }

	protected:

		void Render() const override;

	private:

		std::weak_ptr<Text> m_pTextObj;
		//Text m_TextObj;
		//std::weak_ptr<Text> m_pTextObj;
		//std::weak_ptr<FontAsset> m_pFont;
		//RenderInfo m_RenderTexture;
		//std::string m_Text;
		//glm::fvec4 m_FGColor;
		//glm::fvec4 m_BGColor;
		//uint32_t m_Size;
		//bool m_NeedsToCreateNewTexture;

		//void CreateTexture();
	};
}

