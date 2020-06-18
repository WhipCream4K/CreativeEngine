#include "pch.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "ITextureEntity.h"

dae::Sprite::Sprite()
	: m_TextureRect{}
	, m_Pivot{}
	, m_pTexture{}
	, m_IsFlipX{}
	, m_IsFlipY{}
{
}

dae::Sprite::Sprite(std::weak_ptr<ITextureEntity>&& textureEntity)
	: m_TextureRect{}
	, m_Pivot{}
	, m_pTexture{ std::move(textureEntity) }
	, m_IsFlipX{}
	, m_IsFlipY{}
{
	SetDefaultBoundingAndPivot(m_pTexture.lock()->GetActualDimension());
}

auto dae::Sprite::Create(const std::string& texturePath, const std::string& textureName) -> std::shared_ptr<Sprite>
{
	const auto& texture{ ResourceManager::Load<DefaultTextureData>(texturePath,textureName) };
	auto sprite{ std::make_shared<Sprite>(texture) };
	return sprite;
}

auto dae::Sprite::Create(std::weak_ptr<ITextureEntity>&& textureEntity) -> std::shared_ptr<Sprite>
{
	auto sprite{ std::make_shared<Sprite>(textureEntity.lock()) };
	return sprite;
}

auto dae::Sprite::CreateSpriteSheet(std::weak_ptr<ITextureEntity> texture, const glm::fvec2& minBound,
	const glm::fvec2& dimension, uint32_t amount, float pixelGap,
	SpriteFlow flow) -> std::vector<std::shared_ptr<Sprite>>
{
	std::vector<std::shared_ptr<Sprite>> container{ amount };
	glm::fvec2 subMinBound{ minBound.x,minBound.y };
	const bool leftToRight{ flow == SpriteFlow::LeftToRight };

	for (uint32_t i = 1; i <= amount; ++i)
	{
		auto subSprite{ std::make_shared<Sprite>(texture.lock()) };

		subSprite->SetDimension(dimension, true);
		subSprite->SetSubTextureMinBounding(subMinBound);

		subMinBound.x += (dimension.x + pixelGap) * leftToRight;
		subMinBound.y += (dimension.y + pixelGap) * !leftToRight;

		container[i - 1] = std::move(subSprite);
	}

	return container;
}

auto dae::Sprite::CreateSpriteSheet(const std::string& texturePath, const std::string& textureName,
	const glm::fvec2& minBound, const glm::fvec2& dimension, uint32_t amount, float pixelGap,
	SpriteFlow flow) -> std::vector<std::shared_ptr<Sprite>>
{
	const auto& texture{ ResourceManager::Load<DefaultTextureData>(texturePath,textureName) };
	std::vector<std::shared_ptr<Sprite>> container{ amount };
	glm::fvec2 subMinBound{ minBound.x,minBound.y };
	const bool leftToRight{ flow == SpriteFlow::LeftToRight };

	for (uint32_t i = 1; i <= amount; ++i)
	{
		auto subSprite{ std::make_shared<Sprite>(texture) };

		subSprite->SetDimension(dimension, true);
		subSprite->SetSubTextureMinBounding(subMinBound);

		subMinBound.x += (dimension.x + pixelGap) * leftToRight;
		subMinBound.y += (dimension.y + pixelGap) * !leftToRight;

		container[i - 1] = std::move(subSprite);
	}

	return container;
}

glm::fvec2 dae::Sprite::GetDimension() const
{
	return glm::fvec2{ m_TextureRect.z,m_TextureRect.w };
}

void dae::Sprite::SetSubTextureMinBounding(const glm::fvec2& pos)
{
	m_TextureRect.x = pos.x;
	m_TextureRect.y = pos.y;
}

void dae::Sprite::SetDimension(const glm::fvec2& dimension, bool resetPivot)
{
	m_TextureRect.z = dimension.x;
	m_TextureRect.w = dimension.y;

	if (resetPivot)
	{
		m_Pivot.x = dimension.x / 2.0f;
		m_Pivot.y = dimension.y / 2.0f;
	}
}

void dae::Sprite::SetDefaultBoundingAndPivot(const glm::fvec2& textureDimension)
{
	// Default pivot point
	m_Pivot.x = textureDimension.x / 2.0f;
	m_Pivot.y = textureDimension.y / 2.0f;

	// Default Bounding Box (entire texture)
	m_TextureRect = { 0.0f,0.0f,textureDimension.x,textureDimension.y };
}

void dae::Sprite::Render(const TextureInfo& textureInfo, const RenderTransform& transform) const
{
	m_pTexture.lock()->Render(textureInfo, transform);
}
