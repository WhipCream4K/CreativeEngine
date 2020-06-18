#include "pch.h"
#include "TextRenderer.h"
#include "Renderer.h"

//void dae::TextRenderer::SetFontAsset(std::weak_ptr<FontAsset> pFontAsset)
//{
//	m_NeedsToCreateNewTexture = true;
//	m_pFont = std::move(pFontAsset);
//}
//
//void dae::TextRenderer::SetFontAsset(const std::string& fontName)
//{
//	const auto fontAsset = ResourceManager::GetAsset<FontAsset>(fontName);
//	if (fontAsset)
//	{
//		m_pFont = fontAsset;
//		m_NeedsToCreateNewTexture = true;
//	}
//}
//
//void dae::TextRenderer::SetText(const std::string& text)
//{
//	m_Text = text;
//	m_NeedsToCreateNewTexture = true;
//}
//
//void dae::TextRenderer::SetSize(uint32_t value)
//{
//	m_NeedsToCreateNewTexture = true;
//	m_Size = value;
//}
//
//void dae::TextRenderer::SetForegroundColor(const glm::fvec4& color)
//{
//	m_FGColor = Saturate(color);
//	m_NeedsToCreateNewTexture = true;
//}

auto dae::TextRenderer::SetText(const std::string& text) noexcept -> void
{
	m_pTextObj.lock()->SetText(text);
}

auto dae::TextRenderer::SetText(std::weak_ptr<Text>&& text) noexcept -> void
{
	m_pTextObj = std::move(text);
}

auto dae::TextRenderer::SetFontAsset(std::weak_ptr<IFontEntity>&& fontAsset) noexcept -> void
{
	m_pTextObj.lock()->SetFontAsset(std::move(fontAsset));
}

auto dae::TextRenderer::SetTextSize(uint32_t value) noexcept -> void
{
	m_pTextObj.lock()->SetTextSize(value);
}

auto dae::TextRenderer::SetForegroundColor(const glm::fvec4& color) noexcept -> void
{
	m_pTextObj.lock()->SetForegroundColor(color);
}

auto dae::TextRenderer::SetBackgroundColor(const glm::fvec4& color) noexcept -> void
{
	m_pTextObj.lock()->SetBackgroundColor(color);
}

void dae::TextRenderer::Render() const
{
	// Create origin of the texture for sdl renderer
	//const auto& objectPosition{ GetGameObject()->GetTransform().GetPosition() };
	//const auto& objectScale{ GetGameObject()->GetTransform().GetScale() };
	//const auto& rotation{ GetGameObject()->GetTransform().GetRotation().z };
	//const auto& textureDimension{ m_RenderTexture.GetDimension() };
	//const glm::fvec2 pivot{ textureDimension.x / 2.0f,textureDimension.y / 2.0f };
	//const float depth{ objectPosition.z };

	const auto& gameObjTransform{ GetGameObject()->GetTransform() };
	const RenderTransform transform{ gameObjTransform };
	Renderer::AssignRenderQueue(m_pTextObj, TextureInfo(),transform, gameObjTransform.GetPosition().z);
}
