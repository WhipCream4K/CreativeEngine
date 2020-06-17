#include "pch.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Sprite.h"

auto dae::SpriteRenderer::SetSprite(std::weak_ptr<Sprite>&& sprite) noexcept -> void
{
	m_pSprite = std::move(sprite);
}

auto dae::SpriteRenderer::SetPivotPoint(const glm::fvec2& point) noexcept -> void
{
	m_pSprite.lock()->SetPivotPoint(point);
}

auto dae::SpriteRenderer::SetFlipX(bool value) noexcept -> void
{
	m_pSprite.lock()->SetFlipX(value);
}

auto dae::SpriteRenderer::SetFlipY(bool value) noexcept -> void
{
	m_pSprite.lock()->SetFlipY(value);
}

void dae::SpriteRenderer::Render() const
{
	//const auto texture{ m_Sprite.GetTextureData() };
	//const auto& objectPos{ GetGameObject()->GetTransform().GetPosition() };
	//const auto& objectScale{ GetGameObject()->GetTransform().GetScale() };
	//const float depth{ objectPos.z };
	//const float rotation{ GetGameObject()->GetTransform().GetRotation().z };


	//Renderer::AssignRenderQueue(RenderInfo(desc), m_Sprite.GetTextureData(), depth;
	//Renderer::AssignRenderQueue(, 1.0f);
	
	const auto& gameObjTransform{ GetGameObject()->GetTransform() };
	const RenderTransform transform{ gameObjTransform };
	Renderer::AssignRenderQueue(m_pSprite, transform, gameObjTransform.GetPosition().z);
	//Renderer::AssignRenderQueue(m_pSprite, transform, gameObjTransform.GetPosition().z);
	//Renderer::AssignRenderQueue(&m_Sprite, transform, 0.0f);
}
