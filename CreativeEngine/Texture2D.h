#pragma once
#include "BaseAsset.h"

struct SDL_Texture;
namespace dae
{
	class ITextureEntity;
	class Texture2D final
	{
	public:

		Texture2D(const std::string& assetPath, const std::string& assetName);

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator=(Texture2D&&) = delete;

		// return type of the default render format
		static std::shared_ptr<ITextureEntity> Create(const std::string&, const std::string&);

		std::shared_ptr<DefaultTextureData> GetTextureData();
		//const RenderInfo& GetRenderTexture() const { return m_RenderTexture; }
		//const glm::fvec2& GetDimension() const { return m_RenderTexture.GetDimension(); }
		
	private:

		std::weak_ptr<ITextureEntity> m_DefaultTextureData;
	};
}

