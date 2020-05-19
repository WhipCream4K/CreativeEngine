#pragma once
#include "BaseAsset.h"

struct SDL_Texture;
namespace dae
{
	class Texture2D final : public BaseAsset
	{
	public:

		Texture2D(const std::string& assetPath);
		
		~Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		SDL_Texture* GetSDLTexture() const { return m_pTexture; }
		
	private:
		SDL_Texture* m_pTexture;
	};
}

