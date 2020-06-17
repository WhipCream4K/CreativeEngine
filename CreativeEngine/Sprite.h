#pragma once
#include "IDrawable.h"

struct SDL_Texture;
namespace dae
{
	enum class SpriteFlow
	{
		LeftToRight,
		TopToBottom
	};

	class Texture2D;
	class ITextureEntity;
	class Sprite final : public IDrawable
	{
	public:

		Sprite();
		Sprite(std::weak_ptr<ITextureEntity>&& textureEntity);

#pragma region Possibly Garbage
		
		//static std::shared_ptr<Sprite> CreateSprite(const std::string& assetPath,const std::string& spriteName);
		
		/**
		 * \brief Creates Multiple sprites from a single texture with the given parameters
		 * \param pTexture The texture file
		 * \param minBound The start position of the 1st sprite in pixels
		 * \param dimension The sub-sprite dimension
		 * \param amount Amount of sprite to create
		 * \param flow Flow of the sprite sheet i.e the sprite goes from top to bottom
		 * \return 
		 */
		//static std::shared_ptr<MultiSprites> CreateSpriteSheet(const std::string& assetPath, const std::string& spriteName,
		//	const glm::fvec2& minBound,const glm::fvec2& dimension,
		//	uint32_t amount,float pixelGap = 0.0f,
		//	SpriteFlow flow = SpriteFlow::LeftToRight);

		//static std::shared_ptr<MultiSprites> CreateSpriteSheet(std::weak_ptr<Texture2D> pTexture,
		//	const glm::fvec2& minBound, const glm::fvec2& dimension,
		//	uint32_t amount, float pixelGap = 0.0f,
		//	SpriteFlow flow = SpriteFlow::LeftToRight);
		//	
#pragma endregion

		//static Sprite Create(const std::string& texturePath, const std::string& textureName);
		static auto Create(const std::string& texturePath, const std::string& textureName)->std::shared_ptr<Sprite>;
		static auto Create(std::weak_ptr<ITextureEntity>&& textureEntity)->std::shared_ptr<Sprite>;
		
		//static std::vector<Sprite> CreateSpriteSheet(std::weak_ptr<ITextureEntity> texture, const glm::fvec2& minBound,
		//	const glm::fvec2& dimension, uint32_t amount, float pixelGap, SpriteFlow flow = SpriteFlow::LeftToRight);

		static auto CreateSpriteSheet(std::weak_ptr<ITextureEntity> texture, const glm::fvec2& minBound,
			const glm::fvec2& dimension, uint32_t amount, float pixelGap, 
			SpriteFlow flow = SpriteFlow::LeftToRight) ->std::vector<std::shared_ptr<Sprite>>;

		//static std::vector<Sprite> CreateSpriteSheet(const std::string& texturePath, const std::string& textureName,
		//	const glm::fvec2& minBound, const glm::fvec2& dimension, uint32_t amount, float pixelGap,
		//	SpriteFlow flow = SpriteFlow::LeftToRight);

		static auto CreateSpriteSheet(const std::string& texturePath, const std::string& textureName,
			const glm::fvec2& minBound, const glm::fvec2& dimension, uint32_t amount, float pixelGap,
			SpriteFlow flow = SpriteFlow::LeftToRight) -> std::vector<std::shared_ptr<Sprite>>;
		
		// Getter
		std::shared_ptr<ITextureEntity> GetTextureData() const { return m_pTexture.lock(); }
		const glm::fvec2& GetPivotPoint() const { return m_Pivot; }
		glm::fvec2 GetDimension() const;
		const glm::fvec4& GetTextureRect() const { return m_TextureRect; }
		bool IsFlipX() const { return m_IsFlipX; }
		bool IsFlipY() const { return m_IsFlipY; }

		// Setter
		void SetPivotPoint(const glm::fvec2& pivot) { m_Pivot = pivot; }
		void SetSubTextureMinBounding(const glm::fvec2& pos);
		void SetDimension(const glm::fvec2& dimension,bool resetPivot);
		void SetFlipX(bool flipX) { m_IsFlipX = flipX; }
		void SetFlipY(bool flipY) { m_IsFlipY = flipY; }
		
	private:

		glm::fvec4 m_TextureRect;
		glm::fvec2 m_Pivot;
		std::weak_ptr<ITextureEntity> m_pTexture;
		bool m_IsFlipX;
		bool m_IsFlipY;

		void SetDefaultBoundingAndPivot(const glm::fvec2& textureDimension);
		void Render(const TextureInfo& textureInfo,const RenderTransform& transform) const override;
	};
}

