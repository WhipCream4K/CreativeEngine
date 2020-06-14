#include "pch.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "FontAsset.h"
#include "Renderer.h"

dae::TextRenderer::TextRenderer()
	: m_pFont{}
	, m_RenderTexture{ nullptr }
	, m_Text{}
	, m_FGColor{}
	, m_BGColor{}
	, m_Size{}
	, m_NeedsToCreateNewTexture{}
{
}

//dae::TextRenderer::TextRenderer(std::weak_ptr<GameObject> pGameObject)
//	: BaseComponent(std::move(pGameObject))
//	, m_pFont{}
//	, m_RenderTexture{ nullptr }
//	, m_Text{}
//	, m_FGColor{}
//	, m_BGColor{}
//	, m_Size{}
//	, m_NeedsToCreateNewTexture{}
//{
//}

void dae::TextRenderer::SetFontAsset(std::weak_ptr<FontAsset> pFontAsset)
{
	m_NeedsToCreateNewTexture = true;
	m_pFont = std::move(pFontAsset);
}

void dae::TextRenderer::SetFontAsset(const std::string& fontName)
{
	const auto fontAsset = ResourceManager::GetAsset<FontAsset>(fontName);
	if (fontAsset)
	{
		m_pFont = fontAsset;
		m_NeedsToCreateNewTexture = true;
	}
}

void dae::TextRenderer::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsToCreateNewTexture = true;
}

void dae::TextRenderer::SetSize(uint32_t value)
{
	m_NeedsToCreateNewTexture = true;
	m_Size = value;
}

void dae::TextRenderer::SetForegroundColor(const glm::fvec4& color)
{
	m_FGColor = Saturate(color);
	m_NeedsToCreateNewTexture = true;
}

void dae::TextRenderer::Render() const
{
	// Create origin of the texture for sdl renderer
	const auto& objectPosition{ GetGameObject()->GetTransform().GetPosition() };
	const auto& objectScale{ GetGameObject()->GetTransform().GetScale() };
	const auto& rotation{ GetGameObject()->GetTransform().GetRotation().z };
	const auto& textureDimension{ m_RenderTexture.GetDimension() };
	const glm::fvec2 pivot{ textureDimension.x / 2.0f,textureDimension.y / 2.0f };
	const float depth{ objectPosition.z };

	Renderer::AssignRenderQueue(m_RenderTexture, pivot, objectPosition, objectScale, rotation, depth);
}

void dae::TextRenderer::LateUpdate()
{
	if (m_NeedsToCreateNewTexture)
	{
		CreateTexture();
		m_NeedsToCreateNewTexture = false;
	}
}

void dae::TextRenderer::CreateTexture()
{
	const std::shared_ptr<FontAsset> pFont{ m_pFont };
	if (pFont)
	{
		const auto surface = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(pFont->GetSDLFont(m_Size).get(),
			m_Text.c_str(), Float4ToSDLColor(m_FGColor)),
			SDLDeleter());

		if(m_FGColor.a < 1.0f)
		{
			SDL_SetSurfaceBlendMode(surface.get(), SDL_BLENDMODE_BLEND);
			SDL_SetSurfaceAlphaMod(surface.get(), Uint8(m_FGColor.a * 255.0f));
		}
		
		m_RenderTexture.SetNewSDLTexture(SDL_CreateTextureFromSurface(Renderer::GetSDLRenderer().get(), surface.get()));

	}
}
