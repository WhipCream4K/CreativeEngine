#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

struct SDL_Texture;
namespace dae
{
	class SpriteRenderer final : public BaseComponent
	{
	protected:

		void Render() override;
		void Start() override {}
		void Update() override {}

		void SetSpriteTexture(const std::string& path);
		void SetSpriteTexture(std::shared_ptr<Texture2D> pTexture);

	private:
		
		// This component could own Texture2D on the stack but since
		// I already delete copy constructor so it's really hard to initialize the texture
		// maybe in the future I'll change this
		std::shared_ptr<Texture2D> m_Texture;
	};

}


