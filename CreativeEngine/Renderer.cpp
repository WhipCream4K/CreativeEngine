#include "pch.h"
#include "Renderer.h"

#include <algorithm>

dae::Renderer::~Renderer()
{
}

void dae::Renderer::RenderImpl()
{
	if (m_RenderSurfaces.empty())
		return;

	std::sort(m_RenderSurfaces.begin(), m_RenderSurfaces.end(), [](const RenderSurface& left, const RenderSurface& right)
		{
			return left.depth > right.depth;
		});

	for (const auto& surface : m_RenderSurfaces)
	{
		// TODO: Add scaling options for the texture
		// TODO: Add the option to render part of an image (for Animator)

		if (SDL_RenderCopyEx(m_pRenderer.get(),
			surface.renderTexture.lock().get(),
			&surface.srcRect, &surface.surfaceRect,
			double(surface.rotation),
			&surface.pivot,
			SDL_FLIP_NONE))
			throw std::runtime_error(std::string("Renderer::Render() Failed to render texture ") + SDL_GetError());

		//else
		//{
		//	if (SDL_RenderCopy(m_pRenderer.get(),
		//		surface.renderTexture.lock().get(),
		//		nullptr,
		//		&surface.surfaceRect))
		//		throw std::runtime_error(std::string("Renderer::Render() Failed to render texture ") + SDL_GetError());
		//}
	}

	m_RenderSurfaces.clear();
}

void dae::Renderer::RenderImmediateImpl(const RenderTexture& renderTexture, const glm::fvec2& position,
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

void dae::Renderer::AssignRenderQueueImpl(const RenderTexture& renderTexture, const glm::fvec2& pivot, const glm::fvec2& position,
	const glm::fvec2& scale, float rotation, float depth)
{

	// the default pivot point of the sprite is at the top left of the texture
	// but in this engine I'm going to change to the center of the texture instead
	// |------------------------|
	// |			|			|	
	// |			|			|	
	// |-----------	+ ----------|				
	// |			|			|		
	// |			|			|
	// |------------------------|

	const auto totalTextureDimension{ renderTexture.GetDimension() * scale };
	const auto textureRect{ EngineRectToSDLRect(position, totalTextureDimension.x, totalTextureDimension.y) };

	RenderSurface renderSurface{};
	renderSurface.renderTexture = renderTexture.GetTexture();
	renderSurface.pivot = {int(pivot.x),int(pivot.y)};
	renderSurface.surfaceRect = textureRect;
	renderSurface.srcRect = { 0,0,int(totalTextureDimension.x),int(totalTextureDimension.y) };
	//renderSurface.surfaceRect.x = int(sdlRenderOrigin.x);
	//renderSurface.surfaceRect.y = int(sdlRenderOrigin.y);
	//renderSurface.surfaceRect.w = int(totalTextureDimension.x);
	//renderSurface.surfaceRect.h = int(totalTextureDimension.y);
	renderSurface.depth = depth;
	renderSurface.rotation = rotation;

	m_RenderSurfaces.emplace_back(renderSurface);
}

void dae::Renderer::AssignRenderQueueImpl(const RenderTexture& renderTexture, const glm::fvec4& boundingBox,
	const glm::fvec2& pivot, const glm::fvec2& position, const glm::fvec2& scale, float rotation, float depth)
{
	const glm::fvec2 textureDimension{(boundingBox.z - boundingBox.x),(boundingBox.w - boundingBox.y)};
	const glm::fvec2 totalTextureDimension{ textureDimension * scale };
	const auto textureRect{ EngineRectToSDLRect(position,totalTextureDimension.x,totalTextureDimension.y) };
	
	RenderSurface renderSurface{};
	renderSurface.renderTexture = renderTexture.GetTexture();
	renderSurface.pivot = { int(pivot.x),int(pivot.y) };
	renderSurface.surfaceRect = textureRect;
	renderSurface.depth = depth;
	renderSurface.rotation = rotation;
	renderSurface.srcRect = { int(boundingBox.x),int(boundingBox.y),int(textureDimension.x),int(textureDimension.y) };

	m_RenderSurfaces.emplace_back(renderSurface);
}
