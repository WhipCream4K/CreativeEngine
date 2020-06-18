#include "pch.h"
#include "Renderer.h"
#include "ITextureEntity.h"
#include "IDrawable.h"

#include <algorithm>

void dae::Renderer::RenderImpl()
{
	if (m_RenderTexture.empty())
		return;

	std::sort(m_RenderTexture.begin(), m_RenderTexture.end(), [](const RenderTexture& left, const RenderTexture& right)
		{
			return left.depth > right.depth;
		});

	for (const auto& texture : m_RenderTexture)
	{
		texture.drawObject.lock()->Render(texture.textureInfo,texture.transform);
	}

	m_RenderTexture.clear();
}

void dae::Renderer::RenderImmediateImpl(const RenderInfo& renderTexture, const glm::fvec2& position,
	const glm::fvec2& dimension, float rotation)
{
	renderTexture;
	position;
	dimension;
	rotation;
}

void dae::Renderer::InitializeImpl(std::weak_ptr<SDL_Window> pWindow)
{
	if (pWindow.lock())
		m_pRenderer = std::shared_ptr<SDL_Renderer>(
			SDL_CreateRenderer(pWindow.lock().get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDLDeleter());

	if (!m_pRenderer)
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

void dae::Renderer::AssignRenderQueueImpl(std::weak_ptr<IDrawable>&& drawObject, const RenderTransform& transform,
	float depth)
{
	RenderTexture renderTexture{ std::move(drawObject),TextureInfo(),transform,depth };
	m_RenderTexture.emplace_back(renderTexture);
}

void dae::Renderer::AssignRenderQueueImpl(std::weak_ptr<IDrawable>&& drawObject, const TextureInfo& textureInfo, const RenderTransform& transform, float depth)
{
	RenderTexture renderTexture{std::move(drawObject),textureInfo,transform,depth };
	m_RenderTexture.emplace_back(renderTexture);
}
