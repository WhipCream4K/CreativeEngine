#pragma once

struct SDL_Texture;
namespace dae
{
	class Texture2D final
	{
	public:

		Texture2D() = default;
		~Texture2D();
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&&) = delete;

		static std::shared_ptr<Texture2D> Load(const std::string& path);

		SDL_Texture* GetSDLTexture() { return m_pTexture; }
		
	private:
		SDL_Texture* m_pTexture;
	};
}

