#pragma once


namespace dae
{
	enum class SpriteMode
	{
		SM_Single,
		SM_Multiple
	};

	class Texture2D;
	class Sprite
	{
	public:

		// if sprite mode is multiple then we need to specify the width and the height of a single sprite
		Sprite(SpriteMode spriteMode, float width = 0.0f,float height = 0.0f);
	private:
		std::shared_ptr<Texture2D> m_Texture; // this will hold the "texture" that user will load in
	};
}

