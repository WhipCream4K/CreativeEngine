#pragma once
#include "BaseAsset.h"

struct SDL_Texture;
namespace dae
{
	enum class SamplerState
	{
		SS_PixelSam,
		SS_LinearSam,
		SS_AnisoSam
	};
	
	class Texture2D final : public BaseAsset
	{
	public:

		Texture2D(const std::string& assetPath, const std::string& assetName);
		Texture2D(const std::string& assetPath, const std::string& assetName,SamplerState sampler);

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator=(Texture2D&&) = delete;
		
		static std::shared_ptr<Texture2D> Create(const std::string& assetPath, const std::string& assetName, SamplerState sampler);

		std::shared_ptr<SDL_Texture> GetSDLTexture() const { return m_RenderTexture.GetTexture(); }
		const RenderTexture& GetRenderTexture() const { return m_RenderTexture; }
		const glm::fvec2& GetDimension() const { return m_RenderTexture.GetDimension(); }
		void SetSamplerState(SamplerState sampler);
		

	private:

		RenderTexture m_RenderTexture;
		//SamplerState m_SamplerState;
	};
}

