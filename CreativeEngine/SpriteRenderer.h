#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include "Sprite.h"

struct SDL_Texture;
namespace dae
{
	class Renderer;
	class SpriteRenderer final : public BaseComponent
	{
	public:

		SpriteRenderer(const Sprite& sprite);
		
	protected:

		void Render() override;
		void Start() override {}
		void Update() override {}

	private:
		
		// This component could own Texture2D on the stack but since
		// I already delete copy constructor so it's really hard to initialize the texture
		// maybe in the future I'll change this
		Sprite m_Sprite;
	};

}


