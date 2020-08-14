#include "pch.h"
#include "TextRenderer.h"
#include "Renderer.h"

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
	const auto& gameObjTransform{ GetGameObject()->GetTransform() };
	const RenderTransform transform{ gameObjTransform };
	Renderer::AssignRenderQueue(m_pTextObj, TextureInfo(),transform, gameObjTransform->GetPosition().z);
}
