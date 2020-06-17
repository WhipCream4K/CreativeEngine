#pragma once
#include "ITextureEntity.h"
#include "BaseAsset.h"

namespace dae
{
	struct RenderTransform;
	class SDLTextureData final : public ITextureEntity
	{
	public:

		SDLTextureData(const std::string& assetPath, const std::string& name);
		//static std::shared_ptr<SDLTextureData> Create(const std::string& assetPath, const std::string& assetName,SamplerState sampler);

	private:
		std::shared_ptr<SDL_Texture> m_pTextureData;

		glm::fvec2 GetActualDimensionAfterLoad() const override;
		void Render(const TextureInfo& textureInfo, const RenderTransform& transform, int renderFlags) const override;
		void Init();
	};
}

