#pragma once
#include "EngineAsset.h"

struct SDL_Texture;
namespace dae
{
	enum class SpriteFlow
	{
		LeftToRight,
		TopToBottom
	};

	class Sprite;
	struct MultiSprites final : EngineAsset
	{
		MultiSprites(const std::string& name)
			: EngineAsset(name)
		{
		}

		// well since my other function uses vector<shared_ptr<Sprite>> might as well just manage this on the heap
		std::vector<std::shared_ptr<Sprite>> sprites;
	};
	
	// TODO: Do the animator using this sprite
	class Texture2D;
	class Sprite final : public EngineAsset
	{
	public:

		Sprite(const std::string& spriteName,std::weak_ptr<Texture2D> pTexture);
		// if sprite mode is multiple then we need to specify the width and the height of a single sprite
		//Sprite(std::weak_ptr<Texture2D> pTexture, SpriteMode spriteMode, float width = 0.0f, float height = 0.0f);
		//Sprite(const std::string& assetPath, const std::string& spriteName, SpriteMode spriteMode, float width = 0.0f, float height = 0.0f);
		Sprite(std::weak_ptr<Texture2D> pTexture);
		//Sprite(const std::string& assetPath, const std::string& spriteName);

		static std::shared_ptr<Sprite> CreateSprite(const std::string& assetPath,const std::string& spriteName);
		
		/**
		 * \brief Creates Multiple sprites from a single texture with the given parameters
		 * \param pTexture The texture file
		 * \param minBound The start position of the 1st sprite in pixels
		 * \param dimension The sub-sprite dimension
		 * \param amount Amount of sprite to create
		 * \param flow Flow of the sprite sheet i.e the sprite goes from top to bottom
		 * \return 
		 */
		static std::shared_ptr<MultiSprites> CreateMultiSprite(const std::string& assetPath, const std::string& spriteName,
			const glm::fvec2& minBound,const glm::fvec2& dimension,
			uint32_t amount,float pixelGap = 0.0f,
			SpriteFlow flow = SpriteFlow::LeftToRight);

		static std::shared_ptr<MultiSprites> CreateMultiSprite(std::weak_ptr<Texture2D> pTexture,
			const glm::fvec2& minBound, const glm::fvec2& dimension,
			uint32_t amount, float pixelGap = 0.0f,
			SpriteFlow flow = SpriteFlow::LeftToRight);
		
		std::shared_ptr<Texture2D> GetTexture2D() const { return m_pTexture.lock(); }
		const glm::fvec2& GetPivotPoint() const { return m_Pivot; }
		const glm::fvec2& GetDimension() const { return m_Dimension; }
		const glm::fvec4& GetBoundingBox() const { return m_BoundingBox; }

		void SetPivotPoint(const glm::fvec2& pivot) { m_Pivot = pivot; }
		void SetMinBound(const glm::fvec2& pos);
		void SetMaxBound(const glm::fvec2& pos);
		void SetBoundingBox(const glm::fvec4& box) { m_BoundingBox = box; }
		void SetDimension(const glm::fvec2& dimension) { m_Dimension = dimension; }

	private:

		glm::fvec4 m_BoundingBox;
		glm::fvec2 m_Pivot;
		glm::fvec2 m_Dimension;
		std::weak_ptr<Texture2D> m_pTexture;

		void SetDefaultBoundingAndPivot(const glm::fvec2& textureDimension);
	};


}

