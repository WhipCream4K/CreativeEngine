#include "pch.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include <iostream>
#include <SDL_image.h>


dae::Texture2D::Texture2D(const std::string& assetPath, const std::string& assetName)
	: Texture2D(assetPath, assetName, SamplerState::SS_LinearSam)
{
}

dae::Texture2D::Texture2D(const std::string& assetPath, const std::string& assetName, SamplerState sampler)
	: BaseAsset(assetPath, assetName)
	, m_RenderTexture{ nullptr }
{
	SetSamplerState(sampler);
	m_RenderTexture.SetNewSDLTexture(IMG_LoadTexture(Renderer::GetSDLRenderer().get(), assetPath.c_str()));
}

std::shared_ptr<dae::Texture2D> dae::Texture2D::Create(const std::string& assetPath,
	const std::string& assetName, SamplerState sampler)
{
	auto texture = ResourceManager::Load<Texture2D>(assetPath, assetName);
	texture->SetSamplerState(sampler);
	return texture;
}

void dae::Texture2D::SetSamplerState(SamplerState sampler)
{
	// TODO: Make a logger
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(int(sampler)).c_str()))
		std::cout << std::string("Texture2D::Texture2D(string,string) SDL_SetHint() Error : ") + SDL_GetError() << std::endl;

	m_RenderTexture.SetNewSDLTexture(IMG_LoadTexture(Renderer::GetSDLRenderer().get(), GetPath().c_str()));
}

//void dae::Texture2D::SetSamplerState(SamplerState state)
//{
//	// TODO: Make a logger
//	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, std::to_string(int(state)).c_str()))
//		std::cout << std::string("Texture2D::Texture2D(string,string) SDL_SetHint() Error : ") + SDL_GetError() << std::endl;
//}
