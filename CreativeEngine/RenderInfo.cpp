#include "pch.h"
#include "RenderInfo.h"

//dae::RenderInfo::RenderInfo(SDL_Texture* pSDLTexture)
//	: m_pSDLTexture{}
//	, m_Dimension{}
//{
//	if(pSDLTexture)
//	{
//		m_pSDLTexture = std::shared_ptr<SDL_Texture>(pSDLTexture, SDLDeleter());
//		if (m_pSDLTexture)
//			SetNewDimension(pSDLTexture);
//	}
//}
//
//dae::RenderInfo::RenderInfo(std::weak_ptr<SDL_Texture> pTexture)
//	: RenderInfo(pTexture.lock().get())
//{
//}

//dae::RenderInfo::RenderInfo(const glm::fvec4& textureRect, const glm::fvec2& position, const glm::fvec2& scale, const glm::fvec2& dimension,
//	const glm::fvec2& pivot, float zRotation)
//	: m_TextureRect{}
//	, m_Pos{}
//	, m_Scale{ scale }
//	, m_Pivot{ pivot }
//	, m_Rotation{ zRotation }
//{
//}

dae::RenderInfo::RenderInfo(const RenderInfoDESC& desc)
	: m_TextureRect{ desc.textureRect }
	, m_Pos{ desc.position }
	, m_Scale{ desc.scale }
	, m_Pivot{ desc.pivot }
	, m_Rotation{ desc.zRotation }
	, m_IsFlipX{ desc.isFlipX }
	, m_IsFlipY{ desc.isFlipY }
{
}

glm::fvec2 dae::RenderInfo::GetDimension() const
{
	return glm::fvec2{ m_TextureRect.z - m_TextureRect.x + 1.0f,m_TextureRect.w - m_TextureRect.y + 1.0f };
}

//void dae::RenderInfo::SetNewSDLTexture(SDL_Texture* pSDLTexture)
//{
//	if(pSDLTexture && pSDLTexture != m_pSDLTexture.get())
//	{
//		m_pSDLTexture.reset(pSDLTexture, SDLDeleter());
//		if (m_pSDLTexture)
//			SetNewDimension(pSDLTexture);
//	}
//}
//
//void dae::RenderInfo::SetNewSDLTexture(std::weak_ptr<SDL_Texture> pSDLTexture)
//{
//	const std::shared_ptr<SDL_Texture> tempPtr{ pSDLTexture };
//	
//	if(tempPtr && tempPtr != m_pSDLTexture)
//	{
//		m_pSDLTexture.reset(tempPtr.get(), SDLDeleter());
//		if (m_pSDLTexture)
//			SetNewDimension(tempPtr.get());
//	}
//}
//
//void dae::RenderInfo::SetBlendMode(SDL_BlendMode blendMode)
//{
//	if (SDL_SetTextureBlendMode(m_pSDLTexture.get(), blendMode))
//		throw std::runtime_error(std::string("RenderInfo::SetBlendMode(blendMode) SDL_SetTextureBlendMode Error: ") + SDL_GetError());
//}
//
//void dae::RenderInfo::SetAlphaValue(float value)
//{
//	if (SDL_SetTextureAlphaMod(m_pSDLTexture.get(), Uint8(value * 255.0f)))
//		throw std::runtime_error(std::string("RenderInfo::SetAlphaValue(value) SDL_SetTextureAlphaMod Error: ") + SDL_GetError());
//}
//
//void dae::RenderInfo::SetNewDimension(SDL_Texture* pTexture)
//{
//	int width, height;
//	if (SDL_QueryTexture(pTexture, nullptr, nullptr, &width, &height))
//		throw std::runtime_error(std::string("RenderInfo::SetNewDimension(pTexture) SDL_QueryTexture Error: ") + SDL_GetError());
//
//	m_Dimension.x = float(width);
//	m_Dimension.y = float(height);
//}
