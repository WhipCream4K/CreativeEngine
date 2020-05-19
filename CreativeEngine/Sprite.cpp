#include "pch.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include <SDL.h>

dae::Sprite::Sprite(std::weak_ptr<Texture2D> pTexture, SpriteMode spriteMode, float width, float height)
	: m_Width{}
	, m_Height{}
	, m_pTexture{ pTexture }
	, m_SpriteMode{ spriteMode }
{
	SetSpriteWidthAndHeight(spriteMode, m_pTexture, height, width);
}

dae::Sprite::Sprite(const std::string& assetPath, const std::string& spriteName, SpriteMode spriteMode, float width,
	float height)
	: m_Width{ width }
	, m_Height{ height }
	, m_SpriteMode{ spriteMode }
{
	m_pTexture = ResourceManager::Load<Texture2D>(assetPath, spriteName);
	SetSpriteWidthAndHeight(spriteMode, m_pTexture, height, width);
}

SDL_Texture* dae::Sprite::GetSDLTexture()
{
	return m_pTexture.lock() ? m_pTexture.lock()->GetSDLTexture() : nullptr;
}

void dae::Sprite::SetSpriteWidthAndHeight(SpriteMode spriteMode, std::weak_ptr<Texture2D> pTexture, float height,
	float width)
{
	int q_width, q_height;

	switch (spriteMode)
	{
	case SpriteMode::SM_Single:

		SDL_QueryTexture(pTexture.lock()->GetSDLTexture(), nullptr, nullptr, &q_width, &q_height);
		m_Width = float(q_width);
		m_Height = float(q_height);

		break;
	case SpriteMode::SM_Multiple:

		m_Height = height;
		m_Width = width;

		break;
	}
}
