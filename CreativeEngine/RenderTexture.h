#pragma once

struct SDL_Texture;
namespace dae
{
	class RenderTexture final
	{
	public:

		RenderTexture(SDL_Texture* pSDLTexture);
		RenderTexture(std::weak_ptr<SDL_Texture> pTexture);

		void SetNewSDLTexture(SDL_Texture* pSDLTexture);
		void SetNewSDLTexture(std::weak_ptr<SDL_Texture> pSDLTexture);
		
		const glm::fvec2& GetDimension() const { return m_Dimension; }
		std::shared_ptr<SDL_Texture> GetTexture() const { return m_pSDLTexture; }
		void SetBlendMode(SDL_BlendMode blendMode);
		void SetAlphaValue(float value);

	private:

		std::shared_ptr<SDL_Texture> m_pSDLTexture;
		glm::fvec2 m_Dimension;

		void SetNewDimension(SDL_Texture* pTexture);
	};
}

