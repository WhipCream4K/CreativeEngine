#pragma once
#include "BaseAsset.h"

namespace dae
{
	class Sprite;

	struct TextureInfo
	{
		glm::fvec4 textureRect;
		glm::fvec2 pivot;
		bool isFlipX;
		bool isFlipY;
	};

	struct RenderTransform;
	class ITextureEntity : public BaseAsset
	{
	public:

		ITextureEntity(const std::string& assetPath, const std::string& assetName);
		virtual ~ITextureEntity() = default;
		ITextureEntity(const ITextureEntity&) = delete;
		ITextureEntity& operator=(const ITextureEntity&) = delete;
		ITextureEntity(ITextureEntity&&) = delete;
		ITextureEntity& operator=(ITextureEntity&&) = delete;

		virtual void Render(const TextureInfo&, const RenderTransform&, [[maybe_unused]] int renderFlags = 0) const = 0;
		const glm::fvec2& GetActualDimension();

	protected:

		virtual glm::fvec2 GetActualDimensionAfterLoad() const = 0;

	private:

		glm::fvec2 m_ActualDimension;
		bool m_HasQueryDimension;
	};
}

