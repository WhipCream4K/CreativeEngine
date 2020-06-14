#include "pch.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "ResourceManager.h"


//dae::Sprite::Sprite(std::weak_ptr<Texture2D> pTexture, SpriteMode spriteMode, float width, float height)
//	: m_Pivot{}
//	, m_pTexture{ std::move(pTexture) }
//{
//}
//
//dae::Sprite::Sprite(const std::string& assetPath, const std::string& spriteName, SpriteMode spriteMode, float width,
//	float height)
//	: m_Pivot{}
//	, m_Dimension{}
//	, m_pTexture{}
//{
//	m_pTexture = ResourceManager::Load<Texture2D>(assetPath, spriteName);
//
//	//// Default Pivot point
//	//m_Pivot.x = m_Width / 2.0f;
//	//m_Pivot.y = m_Height / 2.0f;
//}

//dae::Sprite::Sprite(std::weak_ptr<Texture2D> pTexture)
//	: m_BoundingBox{}
//	, m_Pivot{}
//	, m_Dimension{}
//	, m_pTexture(std::move(pTexture))
//{
//	const auto textureDimension = pTexture.lock()->GetDimension();
//	SetDefaultBoundingAndPivot(textureDimension);
//}
//
//dae::Sprite::Sprite(const std::string& assetPath, const std::string& textureName)
//	: m_BoundingBox{}
//	, m_Pivot{}
//	, m_Dimension{}
//	, m_pTexture{}
//{
//	m_pTexture = ResourceManager::Load<Texture2D>(assetPath, textureName);
//	const auto textureDimension = m_pTexture.lock()->GetDimension();
//	SetDefaultBoundingAndPivot(textureDimension);
//}


dae::Sprite::Sprite(const std::string& spriteName, std::weak_ptr<Texture2D> pTexture)
	: EngineAsset(spriteName)
	, m_BoundingBox{}
	, m_Pivot{}
	, m_Dimension{}
	, m_pTexture{ std::move(pTexture) }
{
	const auto textureDimension = m_pTexture.lock()->GetDimension();
	SetDefaultBoundingAndPivot(textureDimension);
}

dae::Sprite::Sprite(std::weak_ptr<Texture2D> pTexture)
	: EngineAsset(pTexture.lock()->GetName())
	, m_BoundingBox{}
	, m_Pivot{}
	, m_Dimension{}
	, m_pTexture{ std::move(pTexture) }
{
	const auto textureDimension = m_pTexture.lock()->GetDimension();
	SetDefaultBoundingAndPivot(textureDimension);
}

//dae::Sprite::Sprite(const std::string& assetPath, const std::string& spriteName)
//	: EngineAsset(spriteName)
//	, m_BoundingBox{}
//	, m_Pivot{}
//	, m_Dimension{}
//	, m_pTexture()
//{
//	m_pTexture = ResourceManager::Load<Texture2D>(assetPath, spriteName);
//	const auto textureDimension{ m_pTexture.lock()->GetDimension() };
//	SetDefaultBoundingAndPivot(textureDimension);
//}

std::shared_ptr<dae::Sprite> dae::Sprite::CreateSprite(const std::string& assetPath, const std::string& spriteName)
{
	const auto& texture{ ResourceManager::Load<Texture2D>(assetPath,spriteName) };
	//TODO: This one should not be make again
	auto sprite{ std::make_shared<Sprite>(texture) };
	ResourceManager::Store(sprite);
	return sprite;
}

std::shared_ptr<dae::MultiSprites> dae::Sprite::CreateMultiSprite(const std::string& assetPath, const std::string& spriteName, const glm::fvec2& minBound,
	const glm::fvec2& dimension, uint32_t amount, float pixelGap, SpriteFlow flow)
{
	const auto& texture{ ResourceManager::Load<Texture2D>(assetPath,spriteName) };

	std::shared_ptr<MultiSprites> multiSprite{ std::make_shared<MultiSprites>(texture->GetName() + "_SubSprites") };

	//std::vector<std::shared_ptr<Sprite>> container{ amount };
	glm::fvec2 subMinBound{ minBound.x,minBound.y };
	glm::fvec2 subMaxBound{ minBound.x + dimension.y,minBound.y + dimension.y };

	for (uint32_t i = 1; i <= amount; ++i)
	{
		const std::string newSuffix{ texture->GetName() + "_" + std::to_string(i) };
		const auto subSprite{ std::make_shared<Sprite>(newSuffix,texture) };
		subSprite->SetDimension(dimension);

		// Left to Right

		subSprite->SetMinBound(subMinBound);
		subSprite->SetMaxBound(subMaxBound);
		subSprite->SetPivotPoint({ dimension.x / 2.0f,dimension.y / 2.0f });

		subMinBound.x += (dimension.x + pixelGap) * int(flow);
		subMaxBound.x += (dimension.x + pixelGap) * int(flow);

		subMinBound.y += (dimension.y + pixelGap) * int(flow);
		subMaxBound.y += (dimension.y + pixelGap) * int(flow);
		
		ResourceManager::Store(multiSprite);
		multiSprite->sprites.push_back(subSprite);
	}

	return multiSprite;
}

std::shared_ptr<dae::MultiSprites> dae::Sprite::CreateMultiSprite(std::weak_ptr<Texture2D> pTexture,
	const glm::fvec2& minBound, const glm::fvec2& dimension, uint32_t amount, float pixelGap, SpriteFlow flow)
{
	flow;
	//std::vector<std::shared_ptr<Sprite>> container{ amount };

	std::shared_ptr<MultiSprites> multiSprite{std::make_shared<MultiSprites>(pTexture.lock()->GetName() + "_SubSprites")};

	const auto texture = pTexture.lock();
	glm::fvec2 subMinBound{ minBound.x,minBound.y };
	glm::fvec2 subMaxBound{ minBound.x + dimension.y,minBound.y + dimension.y };

	for (uint32_t i = 1; i <= amount; ++i)
	{
		const std::string newSuffix{ texture->GetName() + "_" + std::to_string(i) };
		const auto subSprite{ std::make_shared<Sprite>(newSuffix,texture) };
		subSprite->SetDimension(dimension);

		// Left to Right

		subSprite->SetMinBound(subMinBound);
		subSprite->SetMaxBound(subMaxBound);
		subSprite->SetPivotPoint({ dimension.x / 2.0f,dimension.y / 2.0f });

		subMinBound.x += (dimension.x + pixelGap) * int(flow);
		subMaxBound.x += (dimension.x + pixelGap) * int(flow);

		subMinBound.y += (dimension.y + pixelGap) * int(flow);
		subMaxBound.y += (dimension.y + pixelGap) * int(flow);
		
		ResourceManager::Store(multiSprite);
		//container[i - 1] = subSprite;
		multiSprite->sprites.push_back(subSprite);
	}

	
	return multiSprite;
}

void dae::Sprite::SetMinBound(const glm::fvec2& pos)
{
	m_BoundingBox.x = pos.x;
	m_BoundingBox.y = pos.y;
}

void dae::Sprite::SetMaxBound(const glm::fvec2& pos)
{
	m_BoundingBox.z = pos.x;
	m_BoundingBox.w = pos.y;
}

void dae::Sprite::SetDefaultBoundingAndPivot(const glm::fvec2& textureDimension)
{
	// Default pivot point
	m_Pivot.x = textureDimension.x / 2.0f;
	m_Pivot.y = textureDimension.y / 2.0f;

	// Default Bounding Box (entire texture)
	m_BoundingBox = { 0.0f,0.0f,textureDimension.x,textureDimension.y };
}
