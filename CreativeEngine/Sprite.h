#pragma once


struct SDL_Texture;
namespace dae
{
	enum class SpriteMode
	{
		SM_Single,
		SM_Multiple
	};

	// TODO: Do the animator using this sprite
	class Texture2D;
	class Sprite
	{
	public:

		// if sprite mode is multiple then we need to specify the width and the height of a single sprite
		Sprite(std::weak_ptr<Texture2D> pTexture, SpriteMode spriteMode, float width = 0.0f, float height = 0.0f);
		Sprite(const std::string& assetPath, const std::string& spriteName, SpriteMode spriteMode, float width = 0.0f, float height = 0.0f);

		SDL_Texture* GetSDLTexture();
		std::shared_ptr<Texture2D> GetTexture2D() const { return m_pTexture.lock(); }
		SpriteMode GetSpriteMode() const { return m_SpriteMode; }
		float GetHeight() const { return m_Height; }
		float GetWidth() const { return m_Width; }

	private:

		std::weak_ptr<Texture2D> m_pTexture;
		SpriteMode m_SpriteMode;
		float m_Width;
		float m_Height;

		void SetSpriteWidthAndHeight(SpriteMode spriteMode,std::weak_ptr<Texture2D> pTexture,float height,float width);
	};
}

