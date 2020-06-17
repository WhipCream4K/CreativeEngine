#include "pch.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SDLTextureData.h"

#include <iostream>
#include <SDL_image.h>


//dae::Texture2D::Texture2D(const std::string& assetPath, const std::string& assetName)
//	: Texture2D(assetPath, assetName, SamplerState::SS_LinearSam)
//{
//	m_RenderTexture.SetNewSDLTexture(IMG_LoadTexture(Renderer::GetSDLRenderer().get(), assetPath.c_str()));
//}

//void dae::Texture2D::SetSamplerState(SamplerState sampler)
//{
//	// TODO: Make a logger
//	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(int(sampler)).c_str()))
//		std::cout << std::string("Texture2D::Texture2D(string,string) SDL_SetHint() Error : ") + SDL_GetError() << std::endl;
//
//	m_RenderTexture.SetNewSDLTexture(IMG_LoadTexture(Renderer::GetSDLRenderer().get(), GetPath().c_str()));
//}

//void dae::Texture2D::SetSamplerState(SamplerState state)
//{
//	// TODO: Make a logger
//	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(int(state)).c_str()))
//		std::cout << std::string("Texture2D::Texture2D(string,string) SDL_SetHint() Error : ") + SDL_GetError() << std::endl;
//}
std::shared_ptr<dae::ITextureEntity> dae::Texture2D::Create(const std::string& assetPath, const std::string& assetName)
{
	//const auto& textureData{ DefaultTextureData::Create() };
}

std::shared_ptr<dae::DefaultTextureData> dae::Texture2D::GetTextureData()
{
	return std::static_pointer_cast<DefaultTextureData>(m_DefaultTextureData.lock());
}
