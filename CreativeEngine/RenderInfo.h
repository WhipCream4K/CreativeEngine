#pragma once

struct SDL_Texture;
namespace dae
{
	struct RenderInfoDESC
	{
		RenderInfoDESC() = default;
		
		glm::fvec4 textureRect;
		glm::fvec2 position;
		glm::fvec2 scale;
		glm::fvec2 pivot;
		float zRotation;
		bool isFlipX;
		bool isFlipY;
	};

	// TODO: Maybe just change this to struct
	class RenderInfo final
	{
	public:

		//RenderInfo(SDL_Texture* pSDLTexture);
		//RenderInfo(std::weak_ptr<SDL_Texture> pTexture);

		RenderInfo(const RenderInfoDESC& desc);
		
		void SetNewSDLTexture(SDL_Texture* pSDLTexture);
		void SetNewSDLTexture(std::weak_ptr<SDL_Texture> pSDLTexture);
		
		glm::fvec2 GetDimension() const;
		const glm::fvec2& GetPosition() const { return m_Pos; }
		const glm::fvec4& GetTextureRect() const { return m_TextureRect; }
		const glm::fvec2& GetScale() const { return m_Scale; }
		const glm::fvec2& GetPivotPoint() const { return m_Pivot; }
		float GetRotation() const { return m_Rotation; }
		const glm::fvec4& GetForegroundColor() const { return m_FGColor; }
		bool IsFlipX() const { return m_IsFlipX; }
		bool IsFlipY() const { return m_IsFlipY; }
		
		void SetBlendMode(SDL_BlendMode blendMode);
		void SetAlphaValue(float value);

	private:

		glm::fvec4 m_TextureRect;
		glm::fvec4 m_FGColor;
		glm::fvec2 m_Pos;
		glm::fvec2 m_Scale;
		glm::fvec2 m_Pivot;
		float m_Rotation;
		bool m_IsFlipX;
		bool m_IsFlipY;
	};
}

