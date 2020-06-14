#include "pch.h"
#include "RenderTexture.h"

dae::RenderTexture::RenderTexture(SDL_Texture* pSDLTexture)
	: m_pSDLTexture{}
	, m_Dimension{}
{
	if(pSDLTexture)
	{
		m_pSDLTexture = std::shared_ptr<SDL_Texture>(pSDLTexture, SDLDeleter());
		if (m_pSDLTexture)
			SetNewDimension(pSDLTexture);
	}
}

dae::RenderTexture::RenderTexture(std::weak_ptr<SDL_Texture> pTexture)
	: RenderTexture(pTexture.lock().get())
{
}

void dae::RenderTexture::SetNewSDLTexture(SDL_Texture* pSDLTexture)
{
	if(pSDLTexture && pSDLTexture != m_pSDLTexture.get())
	{
		m_pSDLTexture.reset(pSDLTexture, SDLDeleter());
		if (m_pSDLTexture)
			SetNewDimension(pSDLTexture);
	}
}

void dae::RenderTexture::SetNewSDLTexture(std::weak_ptr<SDL_Texture> pSDLTexture)
{
	const std::shared_ptr<SDL_Texture> tempPtr{ pSDLTexture };
	
	if(tempPtr && tempPtr != m_pSDLTexture)
	{
		m_pSDLTexture.reset(tempPtr.get(), SDLDeleter());
		if (m_pSDLTexture)
			SetNewDimension(tempPtr.get());
	}
}

void dae::RenderTexture::SetBlendMode(SDL_BlendMode blendMode)
{
	if (SDL_SetTextureBlendMode(m_pSDLTexture.get(), blendMode))
		throw std::runtime_error(std::string("RenderTexture::SetBlendMode(blendMode) SDL_SetTextureBlendMode Error: ") + SDL_GetError());
}

void dae::RenderTexture::SetAlphaValue(float value)
{
	if (SDL_SetTextureAlphaMod(m_pSDLTexture.get(), Uint8(value * 255.0f)))
		throw std::runtime_error(std::string("RenderTexture::SetAlphaValue(value) SDL_SetTextureAlphaMod Error: ") + SDL_GetError());
}

void dae::RenderTexture::SetNewDimension(SDL_Texture* pTexture)
{
	int width, height;
	if (SDL_QueryTexture(pTexture, nullptr, nullptr, &width, &height))
		throw std::runtime_error(std::string("RenderTexture::SetNewDimension(pTexture) SDL_QueryTexture Error: ") + SDL_GetError());

	m_Dimension.x = float(width);
	m_Dimension.y = float(height);
}
