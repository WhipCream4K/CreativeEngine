#include "pch.h"
#include "SpriteRenderer.h"
#include "Renderer.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Texture2D.h"

//dae::SpriteRenderer::SpriteRenderer()
//	: m_Sprite{}
//{
//	// I can't do this because this needs to happen after shared pointer of this class got created
//	//Renderer::AssignRenderQueue(shared_from_this());
//}

//void dae::SpriteRenderer::SetSprite(const Sprite& sprite)
//{
//	m_Sprite = sprite;
//}

void dae::SpriteRenderer::SetSprite(std::weak_ptr<Sprite> sprite)
{
	m_pSprite = std::move(sprite);
}

void dae::SpriteRenderer::SetPivotPoint(const glm::fvec2& pivot)
{
	m_pSprite.lock()->SetPivotPoint(pivot);
}

void dae::SpriteRenderer::Render() const
{
	const auto texture{ m_pSprite.lock() };
	const auto& objectPos{ GetGameObject()->GetTransform().GetPosition() };
	const auto& objectScale{ GetGameObject()->GetTransform().GetScale() };
	//const glm::fvec2 spriteDimension{ m_Sprite.GetWidth(),m_Sprite.GetHeight() };
	const float depth{ objectPos.z };
	const float rotation{ GetGameObject()->GetTransform().GetRotation().z };

	//Renderer::AssignRenderQueue(texture->GetTexture2D()->GetRenderTexture(), texture->GetPivotPoint(), objectPos, objectScale, rotation, depth);
	Renderer::AssignRenderQueue(texture->GetTexture2D()->GetRenderTexture(), texture->GetBoundingBox(),texture->GetPivotPoint(), objectPos, objectScale, rotation, depth);
}
