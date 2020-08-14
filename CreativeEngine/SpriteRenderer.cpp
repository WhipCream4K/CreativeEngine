#include "pch.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Sprite.h"

auto dae::SpriteRenderer::SetSprite(std::weak_ptr<Sprite>&& sprite, bool reset) noexcept -> void
{
	m_pSprite = std::move(sprite);
	if(reset)
		m_PivotPoint = m_pSprite.lock()->GetPivotPoint();
}

auto dae::SpriteRenderer::SetPivotPoint(const glm::fvec2& point) noexcept -> void
{
	m_PivotPoint = point;
}

auto dae::SpriteRenderer::SetFlipX(bool value) noexcept -> void
{
	m_FlipX = value;
}

auto dae::SpriteRenderer::SetFlipY(bool value) noexcept -> void
{
	m_FlipY = value;
}

void dae::SpriteRenderer::Render() const
{
	const auto& gameObjTransform{ GetGameObject()->GetTransform() };
	const RenderTransform transform{ gameObjTransform };
	const TextureInfo textureInfo{ m_pSprite.lock()->GetTextureRect(),m_PivotPoint,m_FlipX,m_FlipY };
	Renderer::AssignRenderQueue(m_pSprite, textureInfo,transform, gameObjTransform->GetPosition().z);
}
